#include "PreventLosingChaseCamTargetHook.hpp"
#include "Settings.hpp"
#include "State.hpp"

void PreventLosingChaseCamTargetHook::EnableSpecifically(intptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

char PreventLosingChaseCamTargetHook::OverrideFunc(int64_t entity, int64_t point)
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    if (state->IsCharacterMovementMode() && !state->is_in_combat)
    {
        return 0;
    }

    return OriginalFunc(entity, point);
}
