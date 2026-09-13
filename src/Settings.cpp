#pragma comment(lib, "version.lib")

#include "Settings.hpp"
#include "InputconfigPatcher.hpp"
#include "RotationCursorType.hpp"
#include "State.hpp"

using enum ModCommand;

void Settings::Load() noexcept
{
    if (first_time_loaded)
    {
        config.Bind(toggle_walkspeed, "");
        config.Bind(toggle_movement_mode, "key:capslock");
        config.Bind(hold_movement_mode, "");
        config.Bind(toggle_autoforward, "shift+key:w");
        config.Bind(hold_walkspeed, "");
        config.Bind(reload_config, "key:f11");
        config.Bind(toggle_camerarotation, "");

        config.Bind<0.0, 1.0>(walk_speed, 0.3);
        config.Bind(is_walking_default, FALSE);
        config.Bind(walk_after_combat, FALSE);

        config.Bind(auto_toggle_movement_mode, TRUE);
        config.Bind(is_character_movement_mode_default, TRUE);

        config.Bind(rotate_plus_lmb_is_forward, TRUE);
        config.Bind(toggling_movement_mode_also_toggles_camera_rotation, FALSE);
        config.Bind(rotation_cursor_in_character_mode, "invisible");
        config.Bind(rotation_cursor_in_camera_mode, "invisible");

        config.Bind(block_interact_move, FALSE);
        config.Bind(prevent_clicking_active_character, TRUE);
        config.Bind(prevent_clicking_camera_occluders, FALSE);
    }

    auto old_rotation_cursor_in_character_mode = *rotation_cursor_in_character_mode;
    auto old_rotation_cursor_in_camera_mode = *rotation_cursor_in_camera_mode;

    config.Load();

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

    bool should_override_rotation_icon = *rotation_cursor_in_character_mode == "centered" ||
                                         *rotation_cursor_in_camera_mode == "centered";
    state->OverrideRotationCursorIcon(should_override_rotation_icon);
    // We could override the icon everytime camera rotation is pressed or toggled,
    // then character and camera movement modes would be able to have different icons,
    // but I don't think it's necessary and I don't know if it's a performance issue.

    state->rotation_cursor_in_character_mode =
        RotationCursorTypeFromString(*rotation_cursor_in_character_mode);
    state->rotation_cursor_in_camera_mode =
        RotationCursorTypeFromString(*rotation_cursor_in_camera_mode);

    if (!first_time_loaded && state->IsRotating())
    {
        if (old_rotation_cursor_in_character_mode != *rotation_cursor_in_character_mode ||
            old_rotation_cursor_in_camera_mode != *rotation_cursor_in_camera_mode)
        {
            state->RestartCameraRotation();
        }
    }

    first_time_loaded = false;
    INFO("Config loaded successfully."sv)
}

void Settings::InitState()
{
    auto* state = State::GetSingleton();

    state->walking_toggled = is_walking_default;

    state->SetMovementModeToggled(is_character_movement_mode_default);

    // Flag invalid to react later.
    state->cursor_position_to_restore.x = -1;
}

std::vector<std::string> Settings::GetBoundKeycombos(std::string setting)
{
    std::vector<std::string> result = dku::string::split(setting, ","sv);
    return result;
}
