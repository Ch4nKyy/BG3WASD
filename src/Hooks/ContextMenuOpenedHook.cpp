#include "ContextMenuOpenedHook.hpp"
#include "State.hpp"

void ContextMenuOpenedHook::EnableSpecifically(intptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

int64_t ContextMenuOpenedHook::OverrideFunc(int64_t a1, int64_t a2, DWORD* a3)
{
    auto* state = State::GetSingleton();

    state->is_context_menu_open = true;
    state->is_camera_rotation_toggle_dirty = true;

    return OriginalFunc(a1, a2, a3);
}
