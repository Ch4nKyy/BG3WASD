#include "AfterInitialLoadInputConfigHook.hpp"
#include "InputconfigPatcher.hpp"

void AfterInitialLoadInputConfigHook::EnableSpecifically(uintptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

int64_t AfterInitialLoadInputConfigHook::OverrideFunc(int64_t* a1, uint16_t a2)
{
    auto ret = OriginalFunc(a1, a2);

    InputconfigPatcher::Patch();

    return ret;
}
