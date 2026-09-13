#include "DialogStatusChangedHook.hpp"
#include "State.hpp"

void DialogStatusChangedHook::EnableSpecifically(intptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

void DialogStatusChangedHook::OverrideFunc(int64_t some_fat_manager, bool is_in_dialog)
{
    auto* state = State::GetSingleton();

    state->is_in_dialog = is_in_dialog;
    state->is_camera_rotation_toggle_dirty = true;
}
