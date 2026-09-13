#include "ContextMenuClosedHook.hpp"
#include "State.hpp"

void ContextMenuClosedHook::EnableSpecifically(intptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

int64_t ContextMenuClosedHook::OverrideFunc(int64_t a1, int64_t a2, DWORD* a3)
{
    auto* state = State::GetSingleton();

    state->is_context_menu_open = false;
    state->is_camera_rotation_toggle_dirty = true;

    return OriginalFunc(a1, a2, a3);
}
