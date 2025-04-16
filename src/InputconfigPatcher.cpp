#include <algorithm>
#include <format>
#include <shlobj.h>
#include <string>

#include "Hooks/LoadInputConfigHook.hpp"
#include "InputconfigPatcher.hpp"
#include "MessageBox.hpp"
#include "Settings.hpp"
#include "State.hpp"

void InputconfigPatcher::Patch()
{
    try
    {
        ReadAndWriteInputconfig();
        LoadInputConfigHook::CallOriginal(0, 0);
    }
    catch (...)
    {
        WARN(INPUTCONFIG_ERROR);
        MessageBox::Show(INPUTCONFIG_ERROR);
    }
}

bool InputconfigPatcher::IsStringEmptyOrWhitespace(const std::string string)
{
    return string == "" || string == " ";
}

void InputconfigPatcher::ReadAndWriteInputconfig()
{
    auto* state = State::GetSingleton();

    std::wstring config_path_absolute = state->inputconfig_path;

    std::ifstream input_stream(config_path_absolute);
    json data = json::object();
    if (input_stream.good())
    {
        data = json::parse(input_stream);
    }
    input_stream.close();

    data = UpdateData(data, "CharacterMoveBackward", "CameraBackward",
        { "c:leftstick_ypos", "key:s", "key:down" });
    data = UpdateData(data, "CharacterMoveForward", "CameraForward",
        { "c:leftstick_yneg", "key:w", "key:up" });
    data = UpdateData(data, "CharacterMoveLeft", "CameraLeft",
        { "c:leftstick_xneg", "key:a", "key:left" });
    data = UpdateData(data, "CharacterMoveRight", "CameraRight",
        { "c:leftstick_xpos", "key:d", "key:right" });

    data = FixRightclickBinds(data);

    std::ofstream output_stream(config_path_absolute);
    if (output_stream.fail())
    {
        WARN(INPUTCONFIG_ERROR);
        MessageBox::Show(INPUTCONFIG_ERROR);
    }
    output_stream << std::setw(4) << data << std::endl;

    ValidateKeys(data);
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
    }

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

    // TODO character_leftright_is_rotate
    // If this option is set, don't copy camera keys. But preserve Controller keys.
    // if (Settings::GetSingleton()->character_leftright_is_rotate)
    // {
    //     if (character_command == "CharacterMoveLeft")
    //     {
    //         keys_to_bind = { "c:leftstick_xneg" };
    //     }
    //     else if (character_command == "CharacterMoveRight")
    //     {
    //         keys_to_bind = { "c:leftstick_xpos" };
    //     }
    // }

    data[character_command] = keys_to_bind;
    return data;
}

// When binding camera rotate to rightclick, it can collide with the ping/alt key for various
// reasons. In this function we do some tricks to avoid the collision.
// It also collides with modifier keys!
json InputconfigPatcher::FixRightclickBinds(json data)
{
    if (!data.contains("CameraToggleMouseRotate"))
    {
        return data;
    }

    bool rightclick_is_bound_to_rotate = false;
    bool alt_rightclick_is_bound_to_rotate = false;
    bool ctrl_rightclick_is_bound_to_rotate = false;
    bool shift_rightclick_is_bound_to_rotate = false;
    for (json::iterator it = data["CameraToggleMouseRotate"].begin();
        it != data["CameraToggleMouseRotate"].end(); ++it)
    {
        if (*it == "mouse:right")
        {
            rightclick_is_bound_to_rotate = true;
        }
        if (*it == "alt+mouse:right")
        {
            alt_rightclick_is_bound_to_rotate = true;
        }
        if (*it == "ctrl+mouse:right")
        {
            ctrl_rightclick_is_bound_to_rotate = true;
        }
        if (*it == "shift+mouse:right")
        {
            shift_rightclick_is_bound_to_rotate = true;
        }
    }

    if (!rightclick_is_bound_to_rotate)
    {
        for (json::iterator it = data["CameraToggleMouseRotate"].begin();
            it != data["CameraToggleMouseRotate"].end();
            /*inc in loop*/)
        {
            if (*it == "alt+mouse:right" || *it == "ctrl+mouse:right" || *it == "shift+mouse:right")
            {
                it = data["CameraToggleMouseRotate"].erase(it);
            }
            else
            {
                ++it;
            }
        }
        return data;
    }

    // if rightclick is bound to rotate, make sure the modifier combinations are also bound to it
    // to avoid a bug.
    if (!alt_rightclick_is_bound_to_rotate)
    {
        data["CameraToggleMouseRotate"].push_back("alt+mouse:right");
    }
    if (!shift_rightclick_is_bound_to_rotate)
    {
        data["CameraToggleMouseRotate"].push_back("shift+mouse:right");
    }
    if (!ctrl_rightclick_is_bound_to_rotate)
    {
        data["CameraToggleMouseRotate"].push_back("ctrl+mouse:right");
    }

    // remove alt+right from ping to avoid collision.
    // also make sure that ping is not unbound.
    json ping_hotkeys = { "alt+mouse:right" };  // Games default if not changed in the json file.
    if (data.contains("Ping"))
    {
        ping_hotkeys = data["Ping"];
    }
    int valid_keys_bound_to_ping = 0;
    for (json::iterator it = ping_hotkeys.begin(); it != ping_hotkeys.end(); ++it)
    {
        if (dku::string::icontains(*it, "mouse:right"))
        {
            *it = "key:unknown";
        }
        else if (dku::string::icontains(*it, "INVALID:unknown") or
                 dku::string::icontains(*it, "key:unknown"))
        {
        }
        else
        {
            valid_keys_bound_to_ping++;
        }
    }

    if (valid_keys_bound_to_ping == 0)
    {
        ping_hotkeys = { "ctrl+alt+key:f24" };
    }
    data["Ping"] = ping_hotkeys;

    return data;
}

void InputconfigPatcher::UpdateVkCombosOfCommandMap(json data, std::vector<std::string>& commands)
{
    auto* state = State::GetSingleton();
    state->character_forward_keys =
        GetKeycombosOfCommandFromInputconfig(data, "CharacterMoveForward", commands, { "" });
    state->character_backward_keys =
        GetKeycombosOfCommandFromInputconfig(data, "CharacterMoveBackward", commands, { "" });
    state->rotate_keys = GetKeycombosOfCommandFromInputconfig(data, "CameraToggleMouseRotate",
        commands, { "mouse:middle" });
    state->rotate_keys_include_lmb = std::find(state->rotate_keys.begin(), state->rotate_keys.end(),
                                         "mouse:left") != state->rotate_keys.end();
    VirtualKeyMap::UpdateVkCombosOfCommandMap();
}

void InputconfigPatcher::ValidateKeys(json data)
{
    std::vector<std::string> commands;
    UpdateVkCombosOfCommandMap(data, commands);

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
        WARN(error);
        MessageBox::Show(error);
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
            { "ToggleWalkspeed", GetKeycombosAsJsonObject(*settings->toggle_walkspeed) },
            { "ToggleMovementMode", GetKeycombosAsJsonObject(*settings->toggle_movement_mode)},
            { "HoldMovementMode", GetKeycombosAsJsonObject(*settings->hold_movement_mode)},
            { "ToggleAutoforward", GetKeycombosAsJsonObject(*settings->toggle_autoforward)},
            { "HoldWalkspeed", GetKeycombosAsJsonObject(*settings->hold_walkspeed) },
            { "ReloadConfig", GetKeycombosAsJsonObject(*settings->reload_config) }
            // TODO ToggleMouselook
            // { "ToggleMouselook", { *settings->toggle_mouselook } }
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

json InputconfigPatcher::GetKeycombosAsJsonObject(std::string keycombos_as_string)
{
    std::string str = "[";
    for (auto keycombo : dku::string::split(keycombos_as_string, ","sv))
    {
        // Extra escape magic is needed for backslash.
        if (dku::string::icontains(keycombo, "key:\\"))
        {
            str.append(std::format("\"{}\\\",", keycombo));
        }
        else
        {
            str.append(std::format("\"{}\",", keycombo));
        }
    }
    if (str.back() == ',')
    {
        str.pop_back();
    }
    str.append("]");
    return json::parse(str);
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

            if (allow_modifiers && modifiers.size() != 0)
            {
                bool modifiers_all_legal = true;
                for (auto modifier : modifiers)
                {
                    if (modifier != "ctrl" && modifier != "shift" && modifier != "alt")
                    {
                        modifiers_all_legal = false;
                        continue;
                    }
                }
                if (!modifiers_all_legal)
                {
                    not_found_keycombos.push_back(keycombo);
                    continue;
                }
            }

            valid_combos++;
        }

        if (valid_combos == 0)
        {
            unbound_commands.push_back(command);
        }
    }
}
