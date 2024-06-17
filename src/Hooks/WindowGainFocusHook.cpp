#include "WindowGainFocusHook.hpp"
#include "../Settings.hpp"
#include "../State.hpp"
#include "SetVirtualCursorPosHook.hpp"

bool WindowGainFocusHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "E9 ?? ?? ?? ?? 8B ?? ?? 33 ?? 44 ?? ?? ?? 41 ?? ?? 48 ?? ?? 48 ?? ?? ?? 8B ?? 48 ?? ?? ?? "
        "48 ?? ?? 0F ?? ?? ?? ?? ?? 44">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("WindowGainFocusHook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void WindowGainFocusHook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<5>(address, OverrideFunc);
        DEBUG("Hooked WindowGainFocusHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
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
