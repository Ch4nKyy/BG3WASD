#include "State.hpp"
#include "GameCommand.hpp"
#include "InputFaker.hpp"
#include "Patches/BlockCancelActionStoppingMovementPatch.hpp"
#include "Patches/BlockInteractMovePatch.hpp"
#include "SDL.h"
#include "Settings.hpp"

using enum GameCommand;

void State::SetIsRotating(bool in_value)
{
    if (rotate_keys.size() > 0)
    {
        set_is_rotating_was_faked = true;
        if (in_value)
        {
            InputFaker::SendKey(rotate_keys[0], SDL_PRESSED);
        }
        else
        {
            InputFaker::SendKey(rotate_keys[0], SDL_RELEASED);
        }
    }
}

void State::SetInternalIsRotating(bool in_value) { is_rotating = in_value; }

bool State::IsRotating() { return is_rotating; }

void State::HideCursor(bool in_value)
{
    const std::lock_guard<std::mutex> lock(hide_cursor_mutex);
    should_hide_cursor = in_value;
}

bool State::ShouldHideCursor() { return should_hide_cursor; }

void State::EnableInteractMoveBlocker(bool enabled)
{
    if (enabled)
    {
        BlockInteractMovePatch::Enable();
    }
    else
    {
        BlockInteractMovePatch::Disable();
    }
}

/* During Interact move, don't block CancelAction from cancelling movement, or you cannot block it
at all!
But during wasd move, block it, so movement is smoother and Context Menu opens faster after moving.
*/
void State::SetCurrentlyInteractMoving(bool in_value)
{
    if (in_value)
    {
        if (!currently_interact_moving)
        {
            BlockCancelActionStoppingMovementPatch::Disable();
        }
    }
    else
    {
        if (currently_interact_moving)
        {
            BlockCancelActionStoppingMovementPatch::Enable();
        }
    }
    currently_interact_moving = in_value;
}

bool State::IsCurrentlyInteractMoving() { return currently_interact_moving; }

// If set to true, also center camera
void State::SetIsWasdCharacterMovement(bool in_value)
{
    auto* settings = Settings::GetSingleton();

    if (in_value)
    {
        force_stop = true;
        InputFaker::SendCommand(ActionCancel, SDL_RELEASED);
        frames_to_hold_forward_to_center_camera = 10;
    }

    if (*settings->block_interact_move)
    {
        EnableInteractMoveBlocker(in_value);
    }

    // TODO ToggleMouselook
    // if (*settings->toggle_movement_toggles_mouselook)
    // {
    //     SetIsRotating(in_value);
    // }

    is_wasd_character_movement = in_value;
}

bool State::IsWasdCharacterMovement() { return is_wasd_character_movement; }
