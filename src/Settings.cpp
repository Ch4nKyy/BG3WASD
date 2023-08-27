
#include "Settings.hpp"
#include "Addresses/LoadInputConfig.hpp"
#include "InputconfigPatcher.hpp"
#include "State.hpp"
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

            config.Bind<0.0, 1.0>(walk_speed, 0.3);
            config.Bind(walking_is_default, FALSE);

            config.Bind(enable_auto_toggling_wasd_mode, TRUE);
            config.Bind(enable_improved_mouse_rotation, TRUE);
        });

    config.Load();

    if (!loaded_once)
    {
        auto* state = State::GetSingleton();
        state->walking = walking_is_default;
    }
    else
    {
        VirtualKeyMap::UpdateVkCombosOfCommandMap();  // called through inputconfig init hook anyway
    }

    loaded_once = true;
    INFO("Config loaded successfully."sv)
}
