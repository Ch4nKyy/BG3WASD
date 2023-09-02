#include "State.hpp"
#include "InputFaker.hpp"
#include "SDL.h"
#include "Settings.hpp"

void State::SetIsRotating(bool in_value, bool send_fake_key)
{
    is_rotating = in_value;

    // If this is forced, also send a fake key so the game reacts to it.
    if (send_fake_key && rotate_keys.size() > 0)
    {
        if (is_rotating)
        {
            InputFaker::SendKey(rotate_keys[0], SDL_PRESSED);
        }
        else
        {
            InputFaker::SendKey(rotate_keys[0], SDL_RELEASED);
        }
    }
}

bool State::IsRotating() { return is_rotating; }

// If set to true, also center camera
void State::SetIsWasdCharacterMovement(bool in_value)
{
    is_wasd_character_movement = in_value;
    if (Settings::GetSingleton()->toggle_movement_toggles_mouselook)
    {
        SetIsRotating(is_wasd_character_movement);
    }
    if (is_wasd_character_movement)
    {
        frames_to_hold_forward_to_center_camera = 10;
    }
}

bool State::IsWasdCharacterMovement() { return is_wasd_character_movement; }
