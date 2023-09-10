#include "GetInputValueHook.hpp"
#include "../Settings.hpp"
#include "../State.hpp"
#include "../Structs/Vector2.hpp"

bool GetInputValueHook::Prepare()
{
    std::array<uintptr_t, 4> address_array = {
        AsAddress(dku::Hook::Assembly::search_pattern<
            "E8 ?? ?? ?? ?? 44 ?? ?? ?? C7 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 4C ?? ?? ?? ?? ?? ?? ?? "
            "48 ?? ?? 48 ?? ?? ?? ?? F3 ?? ?? ?? ?? ?? E8">()),
        AsAddress(dku::Hook::Assembly::search_pattern<
            "E8 ?? ?? ?? ?? 44 ?? ?? ?? C7 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 4C ?? ?? ?? ?? ?? ?? ?? "
            "48 ?? ?? 48 ?? ?? ?? ?? F3 ?? ?? ?? ?? E8">()),
        AsAddress(dku::Hook::Assembly::search_pattern<
            "E8 ?? ?? ?? ?? 44 ?? ?? ?? C7 ?? ?? ?? ?? ?? ?? ?? 4C ?? ?? ?? ?? 48 ?? ?? 48 ?? ?? "
            "?? ?? F3">()),
        AsAddress(dku::Hook::Assembly::search_pattern<
            "E8 ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? F3 ?? ?? ?? ?? ?? 41 "
            "?? ?? ?? 41">())
    };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("GetInputValueHook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void GetInputValueHook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<5>(address, OverrideFunc);
        INFO("Hooked GetInputValueHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
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
        if (command_id == 142)
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
            if (command_id == 142)
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
    if (not state->IsWasdCharacterMovement())
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
        if (command_id == 142)
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
