#include "AutoRotateUnlockPatch.hpp"

void AutoRotateUnlockPatch::EnableSpecifically(uintptr_t address_incl_offset)
{
    AutoRotateUnlockPatchAsm patch;
    patch.ready();
    auto handle = dku::Hook::AddASMPatch(address_incl_offset, { 0, 7 }, &patch);
    handle->Enable();
    handles.emplace_back(std::move(handle));
}
