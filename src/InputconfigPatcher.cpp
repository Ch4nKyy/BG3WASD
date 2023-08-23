#include <shlobj.h>
#include <string>

#include "InputconfigPatcher.hpp"
#include "Settings.hpp"
#include "State.hpp"

void InputconfigPatcher::Patch()
{
    try
    {
        ReadAndWriteInputconfig();
    }
    catch (...)  // TODO does this catch anything?
    {
        FATAL(
            "Inputconfig could not be patched! "
            "This can often be fixed by renaming or deleting "
            "C:/Users/xxxxx/AppData/Local/Larian Studios/Baldur's Gate "
            "3/PlayerProfiles/Public/inputconfig_p1.json. You can find this folder by pressing "
            "Win+R and typing %localappdata%");
    }
}

bool InputconfigPatcher::IsStringEmptyOrWhitespace(const std::string string)
{
    return string == "" || string == " ";
}

void InputconfigPatcher::ReadAndWriteInputconfig()
{
    wchar_t* localAppDataFolder_wchar;
    if (SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_CREATE, NULL,
            &localAppDataFolder_wchar) != S_OK)
    {
        WARN(
            "Critical error. Could not find LocalAppData. Inputconfig could not be patched! The "
            "mod will probably not work.");
    }

    std::wstring localAppDataFolder = localAppDataFolder_wchar;
    std::wstring config_path_relative =
        L"/Larian Studios/Baldur's Gate 3/PlayerProfiles/Public/inputconfig_p1.json";
    std::wstring config_path_absolute = localAppDataFolder + config_path_relative;

    std::ifstream input_stream(config_path_absolute);
    json data = json::object();
    if (input_stream.good())
    {
        data = json::parse(input_stream);
    }

    auto* settings = Settings::GetSingleton();
    data = UpdateData(data, "CharacterMoveBackward", "CameraBackward",
        { "c:leftstick_yneg", "key:s", "key:down" });
    data = UpdateData(data, "CharacterMoveForward", "CameraForward",
        { "c:leftstick_ypos", "key:w", "key:up" });
    data = UpdateData(data, "CharacterMoveLeft", "CameraLeft",
        { "c:leftstick_xneg", "key:a", "key:left" });
    data = UpdateData(data, "CharacterMoveRight", "CameraRight",
        { "c:leftstick_xpos", "key:d", "key:right" });

    std::ofstream output_stream(config_path_absolute);
    if (output_stream.fail())
    {
        throw std::runtime_error("Can't write file.");
    }
    output_stream << std::setw(4) << data << std::endl;

    auto* state = State::GetSingleton();
    state->character_forward_keys = GetBoundKeysOfCommand(data, "CharacterMoveForward");
    state->character_backward_keys = GetBoundKeysOfCommand(data, "CharacterMoveBackward");
    state->rotate_keys = GetBoundKeysOfCommand(data, "CameraToggleMouseRotate");
    VirtualKeyMap::UpdateVkCombosOfCommandMap();
}

std::vector<std::string> InputconfigPatcher::GetBoundKeysOfCommand(json data, std::string command)
{
    auto key_list = data[command];
    std::vector<std::string> ret_keys;
    for (json::iterator it = key_list.begin(); it != key_list.end(); ++it)
    {
        auto split = dku::string::split(*it, ":"sv);
        if (dku::string::iequals(split[0], "c"))  // ignore controller mappings
        {
            continue;
        }
        if (dku::string::iequals(split[1], "unknown"))
        {
            continue;
        }
        ret_keys.push_back(*it);
    }
    return ret_keys;
}

/*
 Read Camera input key and write it to the Character input key.
 If Camera input key is not set, use default.
*/
json InputconfigPatcher::UpdateData(json data, std::string character_command,
    std::string camera_command, json camera_default_keys)
{
    auto keys_to_bind = camera_default_keys;
    if (data.contains(camera_command))
    {
        keys_to_bind = data[camera_command];
        // Camera and Character input have flipped y axis.
        for (json::iterator it = keys_to_bind.begin(); it != keys_to_bind.end(); ++it)
        {
            if (*it == "c:leftstick_yneg")
            {
                *it = "c:leftstick_ypos";
            }
            else if (*it == "c:leftstick_ypos")
            {
                *it = "c:leftstick_yneg";
            }
        }
    }
    data[character_command] = keys_to_bind;
    return data;
}
