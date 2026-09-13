#include "BlockEdgePanningHook.hpp"
#include "State.hpp"

void BlockEdgePanningHook::EnableSpecifically(intptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

int64_t BlockEdgePanningHook::OverrideFunc(int64_t a1, int16_t a2)
{
    auto* state = State::GetSingleton();

    if (state->IsCharacterMovementMode() && !state->is_in_combat)
    {
        return 0;
    }

    return OriginalFunc(a1, a2);
}

