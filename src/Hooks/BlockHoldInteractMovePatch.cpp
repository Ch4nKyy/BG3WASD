#include "BlockHoldInteractMovePatch.hpp"

void BlockHoldInteractMovePatch::EnableSpecifically(uintptr_t address_incl_offset)
{
    BlockHoldInteractMovePatchAsm patch;
    patch.ready();
    auto handle = dku::Hook::AddASMPatch(address_incl_offset, { 0, 7 }, &patch);
    handles.emplace_back(std::move(handle));
}
