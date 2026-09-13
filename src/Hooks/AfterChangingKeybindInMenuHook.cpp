#include "AfterChangingKeybindInMenuHook.hpp"
#include "InputconfigPatcher.hpp"
#include "State.hpp"

void AfterChangingKeybindInMenuHook::EnableSpecifically(intptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

void AfterChangingKeybindInMenuHook::OverrideFunc(int64_t a1)
{
    OriginalFunc(a1);

    InputconfigPatcher::Patch();
}
