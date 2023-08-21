#include "../State.hpp"
#include "CharacterDeathHook.hpp"

bool CharacterDeathHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "E8 ?? ?? ?? ?? 90 4C ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? 48 ?? ?? E8 ?? ?? ?? ?? 4C ?? ?? 4C ?? "
        "?? ?? 48 ?? ?? ?? 48 ?? ?? E8 ?? ?? ?? ?? 48 ?? ?? 74 ?? BA ?? ?? ?? ?? 48 ?? ?? E8">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            WARN("CharacterDeathHook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void CharacterDeathHook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<5>(address + 57, OverrideFunc);
        INFO("Hooked CharacterDeathHook #{}: {:X}", i, AsAddress(address + 57));
        ++i;
    }
}

bool CharacterDeathHook::OverrideFunc(int64_t a1, int64_t a2)
{
    bool dying_character_is_player = OriginalFunc(a1, a2);

    auto* state = State::GetSingleton();
    state->last_dying_character_is_player = dying_character_is_player;

    return dying_character_is_player;
}
