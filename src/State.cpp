#include "State.hpp"
#include "GameCommand.hpp"
#include "InputFaker.hpp"
#include "Patches/BlockCancelActionStoppingMovementPatch.hpp"
#include "Patches/BlockHoldInteractMovePatch.hpp"
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
    auto* settings = Settings::GetSingleton();

    if (enabled && *settings->block_interact_move)
    {
        BlockInteractMovePatch::Activate();
        BlockHoldInteractMovePatch::Activate();
    }
    else
    {
        BlockInteractMovePatch::Deactivate();
        BlockHoldInteractMovePatch::Deactivate();
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
            BlockCancelActionStoppingMovementPatch::Deactivate();
        }
    }
    else
    {
        if (currently_interact_moving)
        {
            BlockCancelActionStoppingMovementPatch::Activate();
        }
    }
    currently_interact_moving = in_value;
}

bool State::IsCurrentlyInteractMoving() { return currently_interact_moving; }

// If set to true, also center camera
void State::SetCharacterMovementMode(bool in_value)
{
    auto* settings = Settings::GetSingleton();

    if (in_value)
    {
        order_force_stop = true;
        frames_to_hold_forward_to_center_camera = 10;
    }

    EnableInteractMoveBlocker(in_value);

    // TODO ToggleMouselook
    // if (*settings->toggle_movement_toggles_mouselook)
    // {
    //     SetIsRotating(in_value);
    // }

    is_character_movement_mode = in_value;
}

bool State::IsCharacterMovementMode() { return is_character_movement_mode; }
