#include <algorithm>
#include <format>
#include <shlobj.h>
#include <string>

#include "Hooks/LoadInputConfigFunctionRef.hpp"
#include "InputconfigPatcher.hpp"
#include "MessageBox.hpp"
#include "Settings.hpp"
#include "State.hpp"

void InputconfigPatcher::Patch()
{
    try
    {
        ReadAndWriteInputconfig();
        LoadInputConfigFunctionRef::Get()(0, 0);
    }
    catch (...)
    {
        WARN("Caught an exception while patching inputconfig.");
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

    std::ifstream input(config_path_absolute);
    if (!input)
    {
        WARN("Could not open inputconfig file.");
        WARN(INPUTCONFIG_ERROR);
        MessageBox::Show(INPUTCONFIG_ERROR);
        return;
    }
    json data = json::object();
    data = json::parse(input);
    input.close();

    FixInvalidBindings(data);

    UpdateData(data, "CharacterMoveBackward", "CameraBackward",
        { "c:leftstick_ypos", "key:s", "key:down" });
    UpdateData(data, "CharacterMoveForward", "CameraForward",
        { "c:leftstick_yneg", "key:w", "key:up" });
    UpdateData(data, "CharacterMoveLeft", "CameraLeft",
        { "c:leftstick_xneg", "key:a", "key:left" });
    UpdateData(data, "CharacterMoveRight", "CameraRight",
        { "c:leftstick_xpos", "key:d", "key:right" });

    FixRightclickBindings(data);

    if (!WriteJson(config_path_absolute, data))
    {
        WARN(INPUTCONFIG_ERROR);
        MessageBox::Show(INPUTCONFIG_ERROR);
        return;
    }

    ValidateKeys(data);
}

bool InputconfigPatcher::WriteJson(const std::filesystem::path& path, const json& data)
{
    const auto temp_path = path.wstring() + L".tmp";

    {
        std::ofstream output(std::filesystem::path(temp_path), std::ios::trunc);

        if (!output)
        {
            WARN("Failed to open temp file for writing inputconfig.");
            return false;
        }

        output << data.dump(4);

        if (!output)
        {
            WARN("Failed to write to temp file for inputconfig.");
            return false;
        }
    }

    constexpr int max_attempts = 20;

    for (int attempt = 0; attempt < max_attempts; ++attempt)
    {
        if (MoveFileExW(temp_path.c_str(), path.c_str(),
                MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
        {
            return true;
        }

        const DWORD error = GetLastError();

        WARN("Failed to update inputconfig. Attempt #{}. Error: {}", attempt + 1, error);

        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }

    DeleteFileW(temp_path.c_str());

    WARN("Failed to update inputconfig after multiple attempts.");

    return false;
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
void InputconfigPatcher::UpdateData(json& data, const std::string& character_command,
    const std::string& camera_command, const json& camera_default_keys)
{
    auto keys_to_bind = camera_default_keys;

    if (data.contains(camera_command))
    {
        keys_to_bind = data[camera_command];
    }

    // Camera and Character input have flipped y axis.
    for (auto& key : keys_to_bind)
    {
        if (key == "c:leftstick_yneg")
        {
            key = "c:leftstick_ypos";
        }
        else if (key == "c:leftstick_ypos")
        {
            key = "c:leftstick_yneg";
        }
    }

    data[character_command] = keys_to_bind;
}

void InputconfigPatcher::FixInvalidBindings(json& data)
{
    for (auto& [command, keys] : data.items())
    {
        if (!keys.is_array())
            continue;

        auto& array = keys.get_ref<json::array_t&>();

        int unknown_count = 0;

        std::erase_if(array,
            [&unknown_count](const json& key)
            {
                if (key != "INVALID:unknown" && key != "key:unknown")
                    return false;

                ++unknown_count;

                return unknown_count > 2;
            });
    }
}

void InputconfigPatcher::FixRightclickBindings(json& data)
{
    if (!data.contains("CameraToggleMouseRotate"))
    {
        return;
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
            /* inc in loop */)
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
    }

    // If rightclick is bound to rotate, make sure the modifier combinations are also bound to it
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

    // Remove alt+right from ping to avoid collision.
    // Also make sure that ping is not unbound.
    json ping_hotkeys = { "alt+mouse:right" };  // Game's default if not changed in the json file.

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
        else if (!dku::string::icontains(*it, "INVALID:unknown") && 
                 !dku::string::icontains(*it, "key:unknown"))
        {
            valid_keys_bound_to_ping++;
        }
    }

    if (valid_keys_bound_to_ping == 0)
    {
        ping_hotkeys = { "ctrl+alt+key:f24" };
    }

    data["Ping"] = ping_hotkeys;
}

void InputconfigPatcher::UpdateVkCombosOfCommandMap(json data, std::vector<std::string>& commands)
{
    auto* state = State::GetSingleton();

    state->character_forward_keys =
        GetKeycombosOfCommandFromInputconfig(data, "CharacterMoveForward", commands, { "" });

    state->character_backward_keys =
        GetKeycombosOfCommandFromInputconfig(data, "CharacterMoveBackward", commands, { "" });

    auto rotate_keys = GetKeycombosOfCommandFromInputconfig(data, "CameraToggleMouseRotate",
        commands, { "mouse:middle" });
    state->rotate_keys_include_lmb =
        std::find(rotate_keys.begin(), rotate_keys.end(), "mouse:left") != rotate_keys.end();

    VirtualKeyMap::UpdateVkCombosOfCommandMap();
}

void InputconfigPatcher::ValidateKeys(json data)
{
    std::vector<std::string> commands;
    UpdateVkCombosOfCommandMap(data, commands);

    std::vector<std::string> not_found_keycombos;
    FindIssues(data, commands, not_found_keycombos, false);

    std::vector<std::string> mod_hotkeys_not_found_keycombos;
    ValidateModHotkeys(mod_hotkeys_not_found_keycombos);

    if (not_found_keycombos.size() > 0 || mod_hotkeys_not_found_keycombos.size() > 0)
    {
        std::string error = "The following keybinding errors have been detected:\n\n";
        if (not_found_keycombos.size() > 0)
        {
            error.append(
                "The following keys that are bound to game commands could not be mapped. Please "
                "change them and report this issue on Nexusmods:\n");
            for (auto key : not_found_keycombos)
            {
                error.append(key + "\n");
            }
            error.append("\n");
        }
        if (mod_hotkeys_not_found_keycombos.size() > 0)
        {
            error.append(
                "The following keys that are bound to ModHotkeys could not be mapped. Please "
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
            { "ReloadConfig", GetKeycombosAsJsonObject(*settings->reload_config) },
            { "ToggleCameraRotation", GetKeycombosAsJsonObject(*settings->toggle_camerarotation) },
        };
    // clang-format on

    std::vector<std::string> mod_hotkeys;
    for (json::iterator it = data.begin(); it != data.end(); ++it)
    {
        std::string mod_hotkey = it.key();
        mod_hotkeys.push_back(mod_hotkey);
    }

    FindIssues(data, mod_hotkeys, mod_hotkeys_not_found_keycombos, true);
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
    std::vector<std::string>& not_found_keycombos, bool allow_modifiers)
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
    }
}
