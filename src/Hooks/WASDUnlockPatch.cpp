#include "WASDUnlockPatch.hpp"

void WASDUnlockPatch::EnableSpecifically(uintptr_t address_incl_offset)
{
    WASDUnlockPatchAsm patch;
    patch.ready();
    auto handle = dku::Hook::AddASMPatch(address_incl_offset, { 0, 13 }, &patch);
    handle->Enable();
    handles.emplace_back(std::move(handle));
}
