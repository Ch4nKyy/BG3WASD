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
        WARN(
            "Critical error. Inputconfig could not be patched! The mod will "
            "probably not work. This can often be fixed by renaming or deleting "
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
    data = UpdateData(data, settings->backward, settings->backward_fallback, "c:leftstick_yneg",
        "c:leftstick_ypos", "CharacterMoveBackward", "CameraBackward");
    data = UpdateData(data, settings->forward, settings->forward_fallback, "c:leftstick_ypos",
        "c:leftstick_yneg", "CharacterMoveForward", "CameraForward");
    data = UpdateData(data, settings->left, settings->left_fallback, "c:leftstick_xneg",
        "c:leftstick_xneg", "CharacterMoveLeft", "CameraLeft");
    data = UpdateData(data, settings->right, settings->right_fallback, "c:leftstick_xpos",
        "c:leftstick_xpos", "CharacterMoveRight", "CameraRight");

    std::ofstream output_stream(config_path_absolute);
    output_stream << std::setw(4) << data << std::endl;

    auto* state = State::GetSingleton();
    if (state->is_wasd_unlocked)
    {
        if (state->are_extra_features_degraded)
        {
            INFO(
                "Inputconfig patched character controls to main keys and camera controls to "
                "fallback keys.");
        }
        else
        {
            INFO("Inputconfig patched character and camera controls to main keys.");
        }
    }
    else
    {
        INFO("Inputconfig patched camera controls to main keys.");
    }
}

/*
 Bind a third key for the camera to INVALID:unknown, otherwise the game will generate a default
 value.
*/
json InputconfigPatcher::UpdateData(json data, String& key, String& fallback_key,
    std::string controller_key, std::string controller_camera_key, std::string move_command,
    std::string camera_command)
{
    auto* state = State::GetSingleton();

    if (!state->is_wasd_unlocked)
    {
        if (not IsStringEmptyOrWhitespace(*key))
        {
            data[camera_command] = { controller_camera_key, "INVALID:unknown",
                std::format("{}", *key) };
        }
        return data;
    }

    if (state->are_extra_features_degraded)
    {
        if (not IsStringEmptyOrWhitespace(*fallback_key))
        {
            data[move_command] = { controller_key, std::format("{}", *key) };
            data[camera_command] = { controller_camera_key, "INVALID:unknown",
                std::format("{}", *fallback_key) };
        }
        return data;
    }

    if (not IsStringEmptyOrWhitespace(*key))
    {
        data[move_command] = { controller_key, std::format("{}", *key) };
        data[camera_command] = { controller_camera_key, "INVALID:unknown",
            std::format("{}", *key) };
    }

    return data;
}
