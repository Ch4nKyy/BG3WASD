#include "DecideMoveUpdaterHook.hpp"
#include "Settings.hpp"
#include "State.hpp"

void DecideMoveUpdaterHook::EnableSpecifically(uintptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

int64_t DecideMoveUpdaterHook::OverrideFunc(int64_t a1, int64_t a2)
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    // If we are, then this is immediately overwritten in InsideUpdateInteractMove
    state->SetCurrentlyInteractMoving(false);

    return OriginalFunc(a1, a2);
}
