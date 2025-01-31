#include "FTBStartHook.hpp"
#include "Settings.hpp"
#include "State.hpp"

void FTBStartHook::EnableSpecifically(uintptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

int64_t FTBStartHook::OverrideFunc(int64_t a1, int64_t a2, char* a3)
{
    int64_t ret = OriginalFunc(a1, a2, a3);

    if (*Settings::GetSingleton()->enable_auto_toggling_movement_mode)
    {
        auto* state = State::GetSingleton();
        state->SetMovementModeToggled(false);
    }

    return ret;
}
