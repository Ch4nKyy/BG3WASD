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
    catch (...)
    {
        FATAL("Inputconfig could not be patched!");
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
        FATAL("Critical error. Could not find LocalAppData.");
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
        FATAL(
            "Inputconfig could not be patched! "
            "This can often be fixed by renaming or deleting "
            "C:/Users/xxxxx/AppData/Local/Larian Studios/Baldur's Gate "
            "3/PlayerProfiles/Public/inputconfig_p1.json. You can find this folder by pressing "
            "Win+R and typing %localappdata%");
    }
    output_stream << std::setw(4) << data << std::endl;

    UpdateAndValidateKeys(data);
}

std::vector<std::string> InputconfigPatcher::GetKeycombosOfCommandFromInputconfig(json data,
    std::string command, std::vector<std::string>& command_list, json default_keycombos)
{
    command_list.push_back(command);

    auto keycombo_list = default_keycombos;
    if (data.contains(command))
    {
        keycombo_list = data[command];
    }

    std::vector<std::string> ret_keys;
    for (json::iterator it = keycombo_list.begin(); it != keycombo_list.end(); ++it)
    {
        std::string keycombo = *it;
        bool combo_contains_modifier = keycombo.find("+") != std::string::npos;
        if (combo_contains_modifier)
        {
            continue;
        }
        auto split = dku::string::split(keycombo, ":"sv);
        if (dku::string::iequals(split[0], "c"))  // ignore controller mappings
        {
            continue;
        }
        if (dku::string::iequals(split[1], "unknown"))
        {
            continue;
        }
        ret_keys.push_back(keycombo);
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

void InputconfigPatcher::UpdateAndValidateKeys(json data)
{
    std::vector<std::string> commands;

    auto* state = State::GetSingleton();
    state->character_forward_keys =
        GetKeycombosOfCommandFromInputconfig(data, "CharacterMoveForward", commands, { "" });
    state->character_backward_keys =
        GetKeycombosOfCommandFromInputconfig(data, "CharacterMoveBackward", commands, { "" });
    state->rotate_keys = GetKeycombosOfCommandFromInputconfig(data, "CameraToggleMouseRotate",
        commands, { "mouse:middle" });
    VirtualKeyMap::UpdateVkCombosOfCommandMap();

    std::vector<std::string> unbound_commands;
    std::vector<std::string> not_found_keycombos;
    FindIssues(data, commands, unbound_commands, not_found_keycombos, false);

    std::vector<std::string> mod_hotkeys_not_found_keycombos;
    ValidateModHotkeys(mod_hotkeys_not_found_keycombos);

    if (not_found_keycombos.size() > 0 || unbound_commands.size() > 0 ||
        mod_hotkeys_not_found_keycombos.size() > 0)
    {
        std::string error = "The following keybinding errors have been detected:\n\n";
        if (not_found_keycombos.size() > 0)
        {
            error.append(
                "The following keys that are binded to game commands could not be mapped. Please "
                "change them and report this issue on Nexusmods:\n");
            for (auto key : not_found_keycombos)
            {
                error.append(key + "\n");
            }
            error.append("\n");
        }
        if (unbound_commands.size() > 0)
        {
            error.append(
                "The following game commands are not bound to any unmodified key. Please bind the "
                "game commands in the game menu to at least one key that is not modified by "
                "ctrl/shift/alt:\n");
            for (auto command : unbound_commands)
            {
                error.append(command + "\n");
            }
            error.append("\n");
        }
        if (mod_hotkeys_not_found_keycombos.size() > 0)
        {
            error.append(
                "The following keys that are binded to ModHotkeys could not be mapped. Please "
                "change them in the BG3WASD.toml config file:\n");
            for (auto key : mod_hotkeys_not_found_keycombos)
            {
                error.append(key + "\n");
            }
            error.append("\n");
        }
        error.append("This might cause the mod to malfunction!");
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "BG3WASD", error.c_str(), NULL);
    }
}

void InputconfigPatcher::ValidateModHotkeys(
    std::vector<std::string>& mod_hotkeys_not_found_keycombos)
{
    auto* settings = Settings::GetSingleton();
    // Create a fake inputconfig so we can use the validator for ModHotkeys and game Commands.
    // clang-format off
    json data =
        {
            { "ToggleWalkspeed", { *settings->toggle_walkspeed } },
            { "ToggleMovementMode", { *settings->toggle_movement_mode } },
            { "ToggleAutoforward", { *settings->toggle_autoforward } },
            { "HoldWalkspeed", { *settings->hold_walkspeed } },
            { "ReloadConfig", { *settings->reload_config } },
            { "ToggleMouselook", { *settings->toggle_mouselook } }
        };
    // clang-format on

    std::vector<std::string> mod_hotkeys;
    for (json::iterator it = data.begin(); it != data.end(); ++it)
    {
        std::string mod_hotkey = it.key();
        mod_hotkeys.push_back(mod_hotkey);
    }
    std::vector<std::string> mod_hotkeys_unbound;
    FindIssues(data, mod_hotkeys, mod_hotkeys_unbound, mod_hotkeys_not_found_keycombos, true);
}

void InputconfigPatcher::FindIssues(json data, const std::vector<std::string> commands,
    std::vector<std::string>& unbound_commands, std::vector<std::string>& not_found_keycombos,
    bool allow_modifiers)
{
    for (auto command : commands)
    {
        // If command is not in config, then it has default value, which is fine.
        if (!data.contains(command))
        {
            continue;
        }

        json keycombo_list = data[command];
        if (keycombo_list.size() == 0)
        {
            unbound_commands.push_back(command);
            continue;
        }

        int valid_combos = 0;

        for (json::iterator it = keycombo_list.begin(); it != keycombo_list.end(); ++it)
        {
            std::string keycombo = *it;

            auto split = dku::string::split(keycombo, "+"sv);
            auto main_key = split.back();
            split.pop_back();
            auto modifiers = split;

            if (IsStringEmptyOrWhitespace(keycombo))
            {
                continue;
            }
            auto main_key_split = dku::string::split(main_key, ":"sv);
            if (main_key_split.size() < 2)
            {
                not_found_keycombos.push_back(keycombo);
                continue;
            }
            // invalid:unknown and key:unknown can be ignored.
            auto prefix = main_key_split[0];
            auto suffix = main_key_split[1];
            if (suffix == "unknown")
            {
                continue;
            }
            if (prefix == "c")
            {
                continue;
            }

            if (!VirtualKeyMap::VkIsValid(main_key))
            {
                not_found_keycombos.push_back(keycombo);
                continue;
            }

            bool combo_contains_modifier = keycombo.find("+") != std::string::npos;
            if (combo_contains_modifier && !allow_modifiers)
            {
                continue;
            }

            valid_combos++;
        }

        if (valid_combos == 0)
        {
            unbound_commands.push_back(command);
        }
    }
}
