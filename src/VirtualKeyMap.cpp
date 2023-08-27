#include "VirtualKeyMap.hpp"
#include "Settings.hpp"
#include "State.hpp"
#include <algorithm>

int VirtualKeyMap::GetByName(const std::string name)
{
    std::string name_copy = name;
    std::transform(name_copy.begin(), name_copy.end(), name_copy.begin(), ::tolower);
    if (VIRTUAL_KEY_MAP.contains(name_copy))
    {
        return VIRTUAL_KEY_MAP.at(name_copy);
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

void VirtualKeyMap::AddKeyComboForCommand(Command command, std::vector<std::string> setting)
{
    std::vector<std::vector<std::uint32_t>> vkcombos;
    for (auto key : setting)
    {
        auto vkcombo = dku::string::split(key, "+"sv) |
                       std::views::transform(
                           [](auto key_part) { return VirtualKeyMap::GetByName(key_part); }) |
                       std::ranges::to<std::vector<std::uint32_t>>();
        vkcombos.push_back(vkcombo);
    }
    vkcombos_of_command.insert({ command, vkcombos });
}

std::vector<std::vector<std::uint32_t>> VirtualKeyMap::GetVkCombosOfCommand(Command command)
{
    return vkcombos_of_command.at(command);
}

void VirtualKeyMap::UpdateVkCombosOfCommandMap()
{
    auto* settings = Settings::GetSingleton();
    auto* state = State::GetSingleton();
    vkcombos_of_command = {};
    AddKeyComboForCommand(TOGGLE_WALK_OR_SPRINT, std::vector{ *settings->toggle_walk_or_sprint });
    AddKeyComboForCommand(HOLD_WALK_OR_SPRINT, std::vector{ *settings->hold_walk_or_sprint });
    AddKeyComboForCommand(TOGGLE_AUTORUN, std::vector{ *settings->toggle_autorun });
    AddKeyComboForCommand(TOGGLE_CHARACTER_OR_CAMERA,
        std::vector{ *settings->toggle_character_or_camera });
    AddKeyComboForCommand(RELOAD_CONFIG, std::vector{ *settings->reload_config });
    AddKeyComboForCommand(FORWARD, state->character_forward_keys);
    AddKeyComboForCommand(BACKWARD, state->character_backward_keys);
    AddKeyComboForCommand(ROTATE, state->rotate_keys);  // TODO delete here and hook game fct?
    AddKeyComboForCommand(MOUSE_LEFT_DOWN, std::vector{ std::string("mouse:left") });
}
