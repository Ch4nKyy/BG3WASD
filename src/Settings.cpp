
#include "Settings.hpp"
#include "Addresses/LoadInputConfig.hpp"
#include "InputconfigPatcher.hpp"
#include "VirtualKeyMap.hpp"

using enum Command;

void Settings::Load() noexcept
{
    static std::once_flag config_init;
    std::call_once(config_init,
        [&]()
        {
            config.Bind(toggle_walk_or_sprint, "key:insert");
            config.Bind(toggle_character_or_camera, "key:capslock");
            config.Bind(toggle_autorun, "shift+key:w");
            config.Bind(hold_walk_or_sprint, "");
            config.Bind(reload_config, "key:f11");

            /*
             These keys should ship with a default value, so people that don't edit the file
             automatically patch ther inputconfig.
             However, if someone removes the line entirely from the config, then it shouldn't
             default to wasd!
            */
            config.Bind(forward, "");
            config.Bind(backward, "");
            config.Bind(left, "");
            config.Bind(right, "");
            config.Bind(forward_fallback, "");
            config.Bind(backward_fallback, "");
            config.Bind(left_fallback, "");
            config.Bind(right_fallback, "");

            config.Bind<0.0, 1.0>(walk_speed, 0.3);
            config.Bind(walking_is_default, FALSE);
        });

    config.Load();

    LoadVkComboOfCommandMap();

    if (!loaded_once)
    {
        loaded_once = true;
        INFO("Config loaded successfully."sv)
        // During mod init, the InputconfigPatcher is called from the main function, not here,
        // because it might need to react to failed hooks.
        // LoadInputConfig will be called by the game.
    }
    else
    {
        InputconfigPatcher::Patch();
        INFO("Config reloaded successfully."sv)

        if (LoadInputConfig::IsValid())
        {
            LoadInputConfig::Call(0, 0);
            INFO("Inputconfig reloaded successfully."sv)
        }
        else
        {
            WARN("Inputconfig could not be reloaded."sv)
        }
    }
}

void Settings::AddKeyComboForCommand(Command command, String& setting)
{
    auto vkcombo = dku::string::split(*setting, "+"sv) |
                   std::views::transform([](auto key) { return VirtualKeyMap::GetByName(key); }) |
                   std::ranges::to<std::vector<std::uint32_t>>();
    vkcombo_of_command.insert({ command, vkcombo });
}

std::vector<std::uint32_t> Settings::GetVkComboOfCommand(Command command)
{
    return vkcombo_of_command.at(command);
}

void Settings::LoadVkComboOfCommandMap()
{
    vkcombo_of_command = {};
    AddKeyComboForCommand(TOGGLE_WALK_OR_SPRINT, toggle_walk_or_sprint);
    AddKeyComboForCommand(HOLD_WALK_OR_SPRINT, hold_walk_or_sprint);
    AddKeyComboForCommand(TOGGLE_AUTORUN, toggle_autorun);
    AddKeyComboForCommand(TOGGLE_CHARACTER_OR_CAMERA, toggle_character_or_camera);
    AddKeyComboForCommand(RELOAD_CONFIG, reload_config);
    AddKeyComboForCommand(FORWARD, forward);
    AddKeyComboForCommand(BACKWARD, backward);
    AddKeyComboForCommand(LEFT, left);
    AddKeyComboForCommand(RIGHT, right);
}
