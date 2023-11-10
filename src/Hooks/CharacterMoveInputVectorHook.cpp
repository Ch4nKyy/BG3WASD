#include "CharacterMoveInputVectorHook.hpp"
#include "../Settings.hpp"
#include "../State.hpp"
#include "../Structs/Vector3.hpp"

bool CharacterMoveInputVectorHook::Prepare()
{
    std::array<uintptr_t, 3> address_array = {
        AsAddress(dku::Hook::Assembly::search_pattern<
            "E8 ?? ?? ?? ?? F3 ?? ?? ?? ?? ?? F3 ?? ?? ?? ?? ?? F3 ?? ?? ?? ?? ?? F3 ?? ?? ?? ?? "
            "?? ?? ?? F3 ?? ?? ?? F3 ?? ?? ?? F3 ?? ?? ?? F3 ?? ?? ?? F3">()),
        AsAddress(
            dku::Hook::Assembly::search_pattern<"E8 ?? ?? ?? ?? 48 ?? ?? ?? ?? 48 ?? ?? ?? F2 ?? "
                                                "?? ?? F2 ?? ?? ?? ?? 8B ?? ?? 89 ?? ?? E8">()),
        AsAddress(
            dku::Hook::Assembly::search_pattern<"E8 ?? ?? ?? ?? 4C ?? ?? ?? F2 ?? ?? ?? F2">())
    };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("CharacterMoveInputVector #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void CharacterMoveInputVectorHook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<5>(address, OverrideFunc);
        DEBUG("Hooked CharacterMoveInputVector #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

// Called in GameThread, every frame, but not if the player cannot move!
float* CharacterMoveInputVectorHook::OverrideFunc(float* xyz, int64_t a2)
{
    xyz = OriginalFunc(xyz, a2);

    Vector3* xyz_v = reinterpret_cast<Vector3*>(xyz);

    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    if (state->autoforward_toggled ||
        (*settings->enable_rotate_plus_lmb_is_forward && !state->rotate_keys_include_lmb &&
            state->is_mouseleft_pressed && state->IsRotating()))
    {
        // This causes the input vector to not be normalized anymore, but it doesn't matter.
        xyz_v->x = 1.0f;
    }
    if (state->walking_toggled ^ state->walking_held)
    {
        xyz_v->x *= *settings->walk_speed;
        xyz_v->y *= *settings->walk_speed;
        xyz_v->z *= *settings->walk_speed;
    }
    if (not state->IsCharacterMovementMode())
    {
        xyz_v->x = 0.0f;
        xyz_v->y = 0.0f;
        xyz_v->z = 0.0f;
    }
    if (state->frames_to_hold_forward_to_center_camera > 0)
    {
        --(state->frames_to_hold_forward_to_center_camera);
        // Send a move input for a few frames, so the camera moves back to the character.
        // The game has a center camera command, but it sucks, because it always faces north.
        // There is center logic that doesn't do that, e.g. when you press F1, but I didn't find it
        // yet.
        xyz_v->x = 1.0f;
    }

    if (*Settings::GetSingleton()->enable_improved_mouselook)
    {
        state->player_can_input_movement = true;
    }

    return xyz;
}
