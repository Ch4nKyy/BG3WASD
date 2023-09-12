#include "State.hpp"
#include "InputFaker.hpp"
#include "SDL.h"
#include "Settings.hpp"

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

// If set to true, also center camera
void State::SetIsWasdCharacterMovement(bool in_value)
{
    is_wasd_character_movement = in_value;
    // TODO ToggleMouselook
    // if (Settings::GetSingleton()->toggle_movement_toggles_mouselook)
    // {
    //     SetIsRotating(is_wasd_character_movement);
    // }
    if (is_wasd_character_movement)
    {
        frames_to_hold_forward_to_center_camera = 10;
    }
}

bool State::IsWasdCharacterMovement() { return is_wasd_character_movement; }
