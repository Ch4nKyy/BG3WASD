
#include "Settings.hpp"
#include "Addresses/LoadInputConfig.hpp"
#include "InputconfigPatcher.hpp"
#include "State.hpp"

using enum Command;

void Settings::Load() noexcept
{
    static std::once_flag config_init;
    std::call_once(config_init,
        [&]()
        {
            config.Bind(toggle_walkspeed, "key:insert");
            config.Bind(toggle_movement_mode, "key:capslock");
            config.Bind(toggle_autoforward, "shift+key:w");
            config.Bind(hold_walkspeed, "");
            config.Bind(reload_config, "key:f11");
            config.Bind(toggle_mouselook, "");

            config.Bind<0.0, 1.0>(walk_speed, 0.3);
            config.Bind(walking_is_default, FALSE);

            config.Bind(enable_auto_toggling_movement_mode, TRUE);
            
            config.Bind(enable_improved_mouselook, TRUE);
            config.Bind(toggle_movement_toggles_mouselook, FALSE);
            config.Bind(enable_rightclick_mouselook_fix, TRUE);
            config.Bind(rightclick_threshold, 200);
        });

    config.Load();

    if (!*enable_improved_mouselook)
    {
        *toggle_mouselook = "";
        *toggle_movement_toggles_mouselook = false;
    }

    if (!loaded_once)
    {
        InitState();
    }
    // This means the user hot reloaded the toml config!
    else
    {
        InputconfigPatcher::Patch();
    }

    loaded_once = true;
    INFO("Config loaded successfully."sv)
}

void Settings::InitState()
{
    auto* state = State::GetSingleton();

    state->walking_toggled= walking_is_default;

    // Flag invalid to react later.
    state->cursor_position_to_restore.x = -1;
}
