#include "BlockAnalogStickSelection2Patch.hpp"

void BlockAnalogStickSelection2Patch::EnableSpecifically(uintptr_t address_incl_offset)
{
    BlockAnalogStickSelection2PatchAsm patch;
    patch.ready();
    auto handle = dku::Hook::AddASMPatch(address_incl_offset, { 0, 12 }, &patch);
    handle->Enable();
    handles.emplace_back(std::move(handle));
}
