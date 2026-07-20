#include "WindowGainFocusHook.hpp"
#include "SetVirtualCursorPosHook.hpp"
#include "Settings.hpp"
#include "State.hpp"

void WindowGainFocusHook::EnableSpecifically(uintptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

// Called in MainThread, every frame
void WindowGainFocusHook::OverrideFunc()
{
    auto* state = State::GetSingleton();
    if (state->mouselook_toggled)
    {
        state->mouselook_request = ToggleRequest::ON_AND_OFF;
    }
    return;
}
