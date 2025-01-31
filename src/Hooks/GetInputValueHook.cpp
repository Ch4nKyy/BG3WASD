#include "GetInputValueHook.hpp"
#include "GameCommand.hpp"
#include "Settings.hpp"
#include "State.hpp"
#include "Structs/Vector2.hpp"

using enum GameCommand;

void GetInputValueHook::EnableSpecifically(uintptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

// Called in GameThread, every frame, but not if the player cannot move!
int64_t GetInputValueHook::OverrideFunc(int64_t player_input_controller_ptr,
    int64_t in_out_value_vec_ptr, int64_t command_id_ptr, uint8_t a4)
{
    in_out_value_vec_ptr =
        OriginalFunc(player_input_controller_ptr, in_out_value_vec_ptr, command_id_ptr, a4);

    Vector2* xyz_v = reinterpret_cast<Vector2*>(in_out_value_vec_ptr);

    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();
    int command_id = *(int*)command_id_ptr;
    if (state->autoforward_toggled)
    {
        if (command_id == CharacterMoveForward)
        {
            xyz_v->x = 1.0f;
            xyz_v->y = 1.0f;
        }
    }
    if (*settings->enable_rotate_plus_lmb_is_forward && !state->rotate_keys_include_lmb)
    {
        bool rotate_and_lmb_is_pressed = state->is_mouseleft_pressed && state->IsRotating();
        if (state->last_frame_rotate_and_lmb_was_pressed && !rotate_and_lmb_is_pressed)
        {
            state->autoforward_toggled = false;
        }
        if (rotate_and_lmb_is_pressed)
        {
            if (command_id == CharacterMoveForward)
            {
                xyz_v->x = 1.0f;
                xyz_v->y = 1.0f;
            }
        }
        state->last_frame_rotate_and_lmb_was_pressed = rotate_and_lmb_is_pressed;
    }

    if (state->walking_toggled ^ state->walking_held)
    {
        xyz_v->x *= *settings->walk_speed;
        xyz_v->y *= *settings->walk_speed;
    }
    if (not state->IsCharacterMovementMode())
    {
        xyz_v->x = 0.0f;
        xyz_v->y = 0.0f;
    }
    if (state->frames_to_hold_forward_to_center_camera > 0)
    {
        // Send a move input for a few frames, so the camera moves back to the character.
        // The game has a center camera command, but it sucks, because it always faces north.
        // There is center logic that doesn't do that, e.g. when you press F1, but I didn't find it
        // yet.
        if (command_id == CharacterMoveForward)
        {
            --(state->frames_to_hold_forward_to_center_camera);
            xyz_v->x = 1.0f;
            xyz_v->y = 1.0f;
        }
    }

    if (*Settings::GetSingleton()->enable_improved_mouselook)
    {
        state->player_can_input_movement = true;
    }

    return in_out_value_vec_ptr;
}
