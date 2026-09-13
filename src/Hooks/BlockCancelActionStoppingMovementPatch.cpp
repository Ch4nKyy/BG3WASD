#include "BlockCancelActionStoppingMovementPatch.hpp"

void BlockCancelActionStoppingMovementPatch::EnableSpecifically(intptr_t address_incl_offset)
{
    BlockCancelActionStoppingMovementPatchAsm patch;
    patch.ready();
    auto handle = dku::Hook::AddASMPatch(address_incl_offset, { 0, 3 }, &patch);
    handles.emplace_back(std::move(handle));
}
