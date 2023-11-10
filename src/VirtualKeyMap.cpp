#include "VirtualKeyMap.hpp"
#include "Settings.hpp"
#include "State.hpp"
#include <algorithm>

int VirtualKeyMap::GetVkByName(const std::string name)
{
    std::string name_copy = name;
    std::transform(name_copy.begin(), name_copy.end(), name_copy.begin(), ::tolower);
    if (VIRTUAL_KEY_MAP.contains(name_copy))
    {
        return (VIRTUAL_KEY_MAP.at(name_copy))[0];
    }
    std::string key_without_prefix = name_copy.substr(name_copy.find_last_of(":") + 1);
    if (key_without_prefix.length() == 1)
    {
        // Turn a-z to upper case, because the KeyboardProc codes are all upper case.
        // I used toupper() before, but it completely bugged other characters, so I do it manually.
        int code = int(key_without_prefix[0]);
        if (code >= 97 && code <= 122)
        {
            code -= 32;
        }
        return code;
    }
    return 0;
}

int VirtualKeyMap::GetScancodeByName(const std::string name)
{
    std::string name_copy = name;
    std::transform(name_copy.begin(), name_copy.end(), name_copy.begin(), ::tolower);
    if (VIRTUAL_KEY_MAP.contains(name_copy))
    {
        return (VIRTUAL_KEY_MAP.at(name_copy))[1];
    }
    std::string key_without_prefix = name_copy.substr(name_copy.find_last_of(":") + 1);
    if (key_without_prefix.length() == 1)
    {
        int code = int(key_without_prefix[0]);
        // Lower letters
        if (code >= 97 && code <= 122)
        {
            // upper case
            code -= 32;

            code -= 61;
        }
        // Upper letters
        else if (code >= 65 && code <= 90)
        {
            code -= 61;
        }
        // 1 - 9
        else if (code >= 49 && code <= 57)
        {
            code -= 19;
        }
        // 0
        else if (code == 48)
        {
            code = 39;
        }
        return code;
    }
    return 0;
}

int VirtualKeyMap::GetKeycodeByName(const std::string name)
{
    std::string name_copy = name;
    std::transform(name_copy.begin(), name_copy.end(), name_copy.begin(), ::tolower);
    if (VIRTUAL_KEY_MAP.contains(name_copy))
    {
        return (VIRTUAL_KEY_MAP.at(name_copy))[2];
    }
    std::string key_without_prefix = name_copy.substr(name_copy.find_last_of(":") + 1);
    if (key_without_prefix.length() == 1)
    {
        int code = int(key_without_prefix[0]);
        // Upper letters
        if (code >= 65 && code <= 90)
        {
            // lower case
            code += 32;
        }
        return code;
    }
    return 0;
}

void VirtualKeyMap::AddVkCombosForCommand(Command command, std::vector<std::string> keycombos)
{
    std::vector<std::vector<std::uint32_t>> vkcombos;
    for (auto keycombo : keycombos)
    {
        auto vkcombo = dku::string::split(keycombo, "+"sv) |
                       std::views::transform(
                           [](auto key_part) { return VirtualKeyMap::GetVkByName(key_part); }) |
                       std::ranges::to<std::vector<std::uint32_t>>();
        vkcombos.push_back(vkcombo);
    }
    vkcombos_of_command.insert({ command, vkcombos });
}

std::vector<std::vector<std::uint32_t>> VirtualKeyMap::GetVkCombosOfCommand(Command command)
{
    if (vkcombos_of_command.contains(command))
    {
        return vkcombos_of_command.at(command);
    }
    else
    {
        return {};
    }
}

void VirtualKeyMap::UpdateVkCombosOfCommandMap()
{
    auto* settings = Settings::GetSingleton();
    auto* state = State::GetSingleton();
    vkcombos_of_command = {};
    AddVkCombosForCommand(TOGGLE_WALKSPEED,
        settings->GetBoundKeycombos(*settings->toggle_walkspeed));
    AddVkCombosForCommand(HOLD_WALKSPEED, settings->GetBoundKeycombos(*settings->hold_walkspeed));
    AddVkCombosForCommand(TOGGLE_AUTOFORWARD,
        settings->GetBoundKeycombos(*settings->toggle_autoforward));
    AddVkCombosForCommand(TOGGLE_MOVEMENT_MODE,
        settings->GetBoundKeycombos(*settings->toggle_movement_mode));
    AddVkCombosForCommand(HOLD_MOVEMENT_MODE,
        settings->GetBoundKeycombos(*settings->hold_movement_mode));
    AddVkCombosForCommand(RELOAD_CONFIG, settings->GetBoundKeycombos(*settings->reload_config));
    AddVkCombosForCommand(FORWARD, state->character_forward_keys);
    AddVkCombosForCommand(BACKWARD, state->character_backward_keys);
    AddVkCombosForCommand(MOUSE_LEFT_DOWN, settings->GetBoundKeycombos(std::string("mouse:left")));
    // TODO ToggleMouselook
    // AddVkCombosForCommand(TOGGLE_MOUSELOOK, settings->GetBoundKeycombos( *settings->toggle_mouselook ));
}

bool VirtualKeyMap::VkIsValid(std::string keyname) { return GetVkByName(keyname) != 0; }
