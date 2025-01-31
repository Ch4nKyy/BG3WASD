#include "AfterChangingKeybindInMenuHook.hpp"
#include "InputconfigPatcher.hpp"
#include "State.hpp"

void AfterChangingKeybindInMenuHook::EnableSpecifically(uintptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

// Before Patch 7, this was called everytime you changed a keybind, but now it is only called when
// you go back from the keybind tab or when you switch tabs.
void AfterChangingKeybindInMenuHook::OverrideFunc(int64_t a1)
{
    OriginalFunc(a1);

    InputconfigPatcher::Patch();
}
