#include "LootWindowOpenedHook.hpp"
#include "State.hpp"

void LootWindowOpenedHook::EnableSpecifically(intptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

int64_t LootWindowOpenedHook::OverrideFunc(int64_t hash_set, int64_t result, uint64_t* key)
{
    auto* state = State::GetSingleton();

    state->is_loot_window_open = true;
    state->is_camera_rotation_toggle_dirty = true;

    return OriginalFunc(hash_set, result, key);
}
