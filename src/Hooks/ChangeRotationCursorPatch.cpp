#include "ChangeRotationCursorPatch.hpp"

void ChangeRotationCursorPatch::EnableSpecifically(intptr_t address_incl_offset)
{
    ChangeRotationCursorPatchAsm patch;
    patch.ready();
    auto handle = dku::Hook::AddASMPatch(address_incl_offset, { 0, 7 }, &patch);
    handles.emplace_back(std::move(handle));
}
