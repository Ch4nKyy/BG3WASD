#include "State.hpp"
#include "CharacterDeathHook.hpp"

void CharacterDeathHook::EnableSpecifically(uintptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

bool CharacterDeathHook::OverrideFunc(int64_t a1, int64_t a2)
{
    bool dying_character_is_player = OriginalFunc(a1, a2);

    auto* state = State::GetSingleton();
    state->last_dying_character_is_player = dying_character_is_player;

    return dying_character_is_player;
}
