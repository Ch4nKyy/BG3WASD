#include "LootWindowClosedHook.hpp"
#include "State.hpp"

void LootWindowClosedHook::EnableSpecifically(intptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

int64_t LootWindowClosedHook::OverrideFunc(int64_t hash_set, int32_t index)
{
    auto* state = State::GetSingleton();

    auto return_value = OriginalFunc(hash_set, index);

    if (return_value == 0)
    {
        state->is_loot_window_open = false;
        state->is_camera_rotation_toggle_dirty = true;
    }

    return return_value;
}
