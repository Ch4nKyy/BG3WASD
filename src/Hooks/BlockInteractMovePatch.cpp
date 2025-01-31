#include "BlockInteractMovePatch.hpp"

void BlockInteractMovePatch::EnableSpecifically(uintptr_t address_incl_offset)
{
    BlockInteractMovePatchAsm patch;
    patch.ready();
    auto handle = dku::Hook::AddASMPatch(address_incl_offset, { 0, 4 }, &patch);
    handles.emplace_back(std::move(handle));
}
