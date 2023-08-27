#include "CharacterMoveInputVectorHook.hpp"
#include "../Settings.hpp"
#include "../State.hpp"
#include "../Structs/Vector2.hpp"

bool CharacterMoveInputVectorHook::Prepare()
{
    std::array<uintptr_t, 3> address_array = {
        AsAddress(dku::Hook::Assembly::search_pattern<
            "E8 ?? ?? ?? ?? F3 0F ?? ?? F3 0F ?? ?? ?? F3 0F ?? ?? 80 ?? ?? ?? F3">()),
        AsAddress(dku::Hook::Assembly::search_pattern<
            "E8 ?? ?? ?? ?? 0F ?? ?? 48 ?? ?? F3 ?? ?? ?? ?? F3 ?? ?? ?? ?? ?? E8">()),
        AsAddress(dku::Hook::Assembly::search_pattern<
            "E8 ?? ?? ?? ?? F3 0F ?? ?? F3 0F ?? ?? ?? F3 0F ?? ?? F3 0F ?? ?? ?? 80">())
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
        INFO("Hooked CharacterMoveInputVector #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

int64_t CharacterMoveInputVectorHook::OverrideFunc(int64_t yx)
{
    yx = OriginalFunc(yx);

    Vector2* yx_v = reinterpret_cast<Vector2*>(yx);

    auto* state = State::GetSingleton();
    if (state->autorunning || (state->is_mouseleft_pressed && state->is_rotating))
    {
        // This causes the input vector to not be normalized anymore, but it doesn't matter.
        yx_v->y = 1.0f;
    }
    if (state->walking ^ state->walking_or_sprint_held)
    {
        auto* settings = Settings::GetSingleton();
        yx_v->x *= *settings->walk_speed;
        yx_v->y *= *settings->walk_speed;
    }
    if (not state->is_wasd_character_movement)
    {
        yx_v->x = 0.0f;
        yx_v->y = 0.0f;
    }
    if (state->frames_to_hold_forward_to_center_camera > 0)
    {
        --(state->frames_to_hold_forward_to_center_camera);
        // Send a move input for a few frames, so the camera moves back to the character.
        // The game has a center camera command, but it sucks, because it always faces north.
        // There is center logic that doesn't do that, e.g. when you press F1, but I didn't find it
        // yet.
        yx_v->y = 1.0f;
    }

    return yx;
}
