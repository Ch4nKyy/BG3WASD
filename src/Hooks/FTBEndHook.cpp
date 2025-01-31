#include "FTBEndHook.hpp"
#include "Settings.hpp"
#include "State.hpp"

void FTBEndHook::EnableSpecifically(uintptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

int64_t FTBEndHook::OverrideFunc(int64_t a1, int64_t a2, char* a3)
{
    auto* state = State::GetSingleton();

    int64_t ret = OriginalFunc(a1, a2, a3);

    if (*Settings::GetSingleton()->enable_auto_toggling_movement_mode &&
        SDL_GetTicks() - state->last_time_combat_state_changed > 500)
    {
        state->SetMovementModeToggled(true);
    }

    return ret;
}
