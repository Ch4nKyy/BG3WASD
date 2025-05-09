#pragma comment(lib, "version.lib")

#include "Settings.hpp"
#include "Hooks/LoadInputConfigHook.hpp"
#include "InputconfigPatcher.hpp"
#include "State.hpp"

using enum Command;

void Settings::Load() noexcept
{
    if (first_time_loaded)
    {
        config.Bind(toggle_walkspeed, "key:insert");
        config.Bind(toggle_movement_mode, "key:capslock");
        config.Bind(hold_movement_mode, "");
        config.Bind(toggle_autoforward, "shift+key:w");
        config.Bind(hold_walkspeed, "");
        config.Bind(reload_config, "key:f11");
        // TODO ToggleMouselook
        // config.Bind(toggle_mouselook, "");

        config.Bind<0.0, 1.0>(walk_speed, 0.3);
        config.Bind(walking_is_default, FALSE);
        config.Bind(walk_after_combat, FALSE);
        // TODO character_leftright_is_rotate
        // config.Bind(character_leftright_is_rotate, FALSE);
        // Future toml lines:
        // CharacterLeftRightIsRotateInsteadOfMove = false
        // # Set this to true if you want to MOVE the Camera with Left/Right, but ROTATE the
        // # Character.
        // # For this to work, you need to bind Camera Rotate Left/Right to the same keys as
        // # Camera Left/Right.

        config.Bind(enable_auto_toggling_movement_mode, TRUE);

        config.Bind(enable_improved_mouselook, TRUE);
        config.Bind(enable_rotate_plus_lmb_is_forward, TRUE);
        // TODO ToggleMouselook
        // TODO ToggleMouselook
        // config.Bind(toggle_movement_toggles_mouselook, FALSE);
        config.Bind(rotate_threshold, 200);

        config.Bind(block_interact_move, FALSE);
    }

    config.Load();

    if (!*enable_improved_mouselook)
    {
        // TODO ToggleMouselook
        // *toggle_mouselook = "";
        // *toggle_movement_toggles_mouselook = false;
    }

    auto* state = State::GetSingleton();

    if (first_time_loaded)
    {
        InitState();
    }
    else  // reloaded
    {
        // during first load, this is called in AfterInitialLoadInputConfigHook
        InputconfigPatcher::Patch();
    }
    state->EnableInteractMoveBlocker(state->IsCharacterMovementMode());

    first_time_loaded = false;
    INFO("Config loaded successfully."sv)
}

void Settings::InitState()
{
    auto* state = State::GetSingleton();

    state->walking_toggled = walking_is_default;

    // Flag invalid to react later.
    state->cursor_position_to_restore.x = -1;
}

std::vector<std::string> Settings::GetBoundKeycombos(std::string setting)
{
    std::vector<std::string> result = dku::string::split(setting, ","sv);
    return result;
}
