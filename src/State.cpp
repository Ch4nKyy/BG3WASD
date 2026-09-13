#include "State.hpp"
#include "CommandId.hpp"
#include "Hooks/BlockCancelActionStoppingMovementPatch.hpp"
#include "Hooks/BlockHoldInteractMovePatch.hpp"
#include "Hooks/BlockInteractMovePatch.hpp"
#include "Hooks/ChangeRotationCursorPatch.hpp"
#include "InputFaker.hpp"
#include "SDL.h"
#include "Settings.hpp"
#include "Structs/Camera.hpp"

using enum CommandId;

// The actual state of the toggle is is_camera_rotation_toggled,
// but if any window is open that requires user input,
// or if the player is in a dialog or cutscene,
// the camera rotation should be disengaged.
bool State::IsCameraRotationToggled()
{
    return is_camera_rotation_toggled && !IsPlayerInputBlocked();
}

void State::ToggleCameraRotation()
{
    is_camera_rotation_toggled = !is_camera_rotation_toggled;
    is_camera_rotation_toggle_dirty = true;
}

// If the player input is fully or partially blocked by a menu or cutscene,
// block the mod hotkeys to prevent unintended actions.
bool State::IsPlayerInputBlocked()
{
    return is_context_menu_open || is_menu_open || is_in_dialog || IsInMainMenu() ||
           is_in_photomode || is_loot_window_open;
}

// Ask the game to start camera rotation.
void State::EnableCameraRotation(bool in_value)
{
    // Don't add a IsRotating() check here that returns early, because sometimes we have to
    // forcefully set it to the same value to unstuck it.

    camera_rotation_toggled_this_frame = true;
    if (in_value)
    {
        InputFaker::SendCommand(CameraToggleMouseRotate, SDL_PRESSED);
    }
    else
    {
        InputFaker::SendCommand(CameraToggleMouseRotate, SDL_RELEASED);
    }
    camera_rotation_toggled_this_frame = false;
}

// This function itself does not change the camera rotation.
// It is called when the game has actually started or stopped rotating the camera.
// It is used to store the state in the mod.
void State::SetInternalCameraRotationState(bool in_value) { is_camera_rotating = in_value; }

bool State::IsRotating() { return is_camera_rotating; }

void State::OverrideCursor(bool in_value)
{
    const std::lock_guard<std::mutex> lock(override_cursor_mutex);
    should_override_cursor = in_value;
}

bool State::ShouldOverrideCursor() { return should_override_cursor; }

// This patch changes the rotation cursor from the two arrow thingy to a crosshair.
void State::OverrideRotationCursorIcon(bool enabled)
{
    if (enabled)
    {
        ChangeRotationCursorPatch::Get().Activate();
    }
    else
    {
        ChangeRotationCursorPatch::Get().Deactivate();
    }
}

// This patch blocks interact/leftclick movement.
void State::EnableInteractMoveBlocker(bool enabled)
{
    auto* settings = Settings::GetSingleton();

    if (enabled && *settings->block_interact_move)
    {
        BlockInteractMovePatch::Get().Activate();
        BlockHoldInteractMovePatch::Get().Activate();
    }
    else
    {
        BlockInteractMovePatch::Get().Deactivate();
        BlockHoldInteractMovePatch::Get().Deactivate();
    }
}

/* During Interact move, don't block CancelAction from cancelling movement, or you cannot block it
at all!
But during wasd move, block it, so movement is smoother and Context Menu opens faster after moving.
*/
void State::SetCurrentlyInteractMoving(bool in_value) { currently_interact_moving = in_value; }

void State::UpdateBlockCancelActionStoppingMovementPatch()
{
    if (BlockCancelActionStoppingMovementPatch::Get().IsActive())
    {
        if (currently_interact_moving)
        {
            BlockCancelActionStoppingMovementPatch::Get().Deactivate();
        }
    }
    else
    {
        if (!currently_interact_moving)
        {
            BlockCancelActionStoppingMovementPatch::Get().Activate();
        }
    }
}

bool State::IsCurrentlyInteractMoving() { return currently_interact_moving; }

void State::SetMovementModeToggled(bool in_value)
{
    if (GetRotationCursorTypeInCurrentMode() == RotationCursorType::VANILLA && IsRotating() &&
        !is_camera_rotation_toggled)
    {
        dont_restart_camera = true;
    }

    movement_mode_toggled = in_value;

    UpdateMovementMode();
}

void State::SetMovementModeHeld(bool in_value)
{
    if (GetRotationCursorTypeInCurrentMode() == RotationCursorType::VANILLA && IsRotating() &&
        !is_camera_rotation_toggled)
    {
        dont_restart_camera = true;
    }

    movement_mode_held = in_value;

    UpdateMovementMode();
}

bool State::IsCameraValid(Camera* camera)
{
    if (camera->currentZoomA < -0.001f)
    {
        return false;
    }
    return true;
}

Camera* State::GetCamera()
{
    if (!IsCameraValid(camera))
    {
        return nullptr;
    }
    else
    {
        return camera;
    }
}

void State::SetCamera(Camera* in_camera)
{
    if (IsCameraValid(in_camera))
    {
        camera = in_camera;
        last_valid_camera_zoom = in_camera->currentZoomA;
    }
}

void State::OrderForceStop()
{
    is_force_stop = true;
    InputFaker::QueueCommand(CommandId::ActionCancel, SDL_RELEASED);
}

// We need a way to detect whether the player is in the main menu, because is_menu_open does not work
// for the main menu. Checking whether the active character is invalid seems easy and is hopefully reliable.
bool State::IsInMainMenu()
{
    return active_character == 0xFFC000000000000 || active_character == 0xFFC0000000000000;
}

// If current mode is character movement, also center camera on character
void State::UpdateMovementMode()
{
    auto* settings = Settings::GetSingleton();

    bool is_character_movement = IsCharacterMovementMode();

    if (is_character_movement)
    {
        OrderForceStop();
        CenterCamera();
    }

    EnableInteractMoveBlocker(is_character_movement);

    if (IsRotating())
    {
        RestartCameraRotation();
    }

    if (*settings->toggling_movement_mode_also_toggles_camera_rotation)
    {
        bool old_is_camera_rotation_toggled = is_camera_rotation_toggled;
        is_camera_rotation_toggled = is_character_movement;
        if (old_is_camera_rotation_toggled != is_camera_rotation_toggled)
        {
            is_camera_rotation_toggle_dirty = true;
        }
    }
}

void State::RestartCameraRotation()
{
    if (dont_restart_camera == true)
    {
        EnableCameraRotation(false);
        dont_restart_camera = false;
    }
    else
    {
        // Setting is_camera_rotation_toggle_very_dirty is only necessary to avoid a bug where the cursor gets
        // restored in a weird position.
        is_camera_rotation_toggle_very_dirty = true;
        EnableCameraRotation(false);
        EnableCameraRotation(true);
        is_camera_rotation_toggle_very_dirty = false;

        switch (GetRotationCursorTypeInCurrentMode())
        {
        case RotationCursorType::INVISIBLE:
        case RotationCursorType::CENTERED:
            SDL_SetRelativeMouseMode(SDL_FALSE);  // necessary for it to update!
            SDL_SetRelativeMouseMode(SDL_TRUE);
            break;
        case RotationCursorType::VANILLA:
            SDL_SetRelativeMouseMode(SDL_FALSE);
        }
    }
}

void State::CenterCamera() { frames_to_hold_forward_to_center_camera = 10; }

RotationCursorType State::GetRotationCursorTypeInCurrentMode()
{
    if (IsCharacterMovementMode())
    {
        return rotation_cursor_in_character_mode;
    }
    else
    {
        return rotation_cursor_in_camera_mode;
    }
}

bool State::IsCharacterMovementMode() { return movement_mode_toggled ^ movement_mode_held; }

bool State::IsMovementModeToggled() { return movement_mode_toggled; }
