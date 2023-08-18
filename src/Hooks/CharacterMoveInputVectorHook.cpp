#include "CharacterMoveInputVectorHook.hpp"
#include "../Settings.hpp"
#include "../State.hpp"
#include "../Structs/Vector2.hpp"

bool CharacterMoveInputVectorHook::Prepare()
{
    std::array<uintptr_t, 3> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
                      "E8 ?? ?? ?? ?? F3 0F ?? ?? F3 0F ?? ?? ?? F3 0F ?? ?? 80 ?? ?? ?? F3">()),
        AsAddress(dku::Hook::Assembly::search_pattern<
            "E8 ?? ?? ?? ?? 0F ?? ?? 48 ?? ?? F3 ?? ?? ?? ?? F3 ?? ?? ?? ?? ?? E8">()),
        AsAddress(dku::Hook::Assembly::search_pattern<
            "E8 ?? ?? ?? ?? F3 0F ?? ?? F3 0F ?? ?? ?? F3 0F ?? ?? F3 0F ?? ?? ?? 80">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
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
    if (state->autorunning)
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

    return yx;
}
