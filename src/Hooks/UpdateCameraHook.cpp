#include "UpdateCameraHook.hpp"
#include "Addresses/IsInControllerMode.hpp"
#include "Settings.hpp"
#include "State.hpp"

bool UpdateCameraHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "E8 ?? ?? ?? ?? 48 8D 8D E8 04 00 00 E8 ?? ?? ?? ?? E9 A6 FD FF FF">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("UpdateCameraHook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void UpdateCameraHook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<5>(address, OverrideFunc);
        DEBUG("Hooked UpdateCameraHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

// Called in GameThread, every frame? Called before HandleCameraInput.
int64_t UpdateCameraHook::OverrideFunc(uint64_t a1, uint64_t a2, uint64_t a3, int64_t a4)
{
    auto* settings = Settings::GetSingleton();
    auto* state = State::GetSingleton();

    if (IsInControllerMode::Read())
    {
        return OriginalFunc(a1, a2, a3, a4);
    }

    int64_t camera_object_ptr = *(int64_t *)(a4 + 48);

    bool new_combat_state = (*reinterpret_cast<bool*>(camera_object_ptr + 168) & 1) != 0;
    if (!state->combat_state_initiliazed || new_combat_state != state->old_combat_state)
    {
        if (*settings->enable_auto_toggling_movement_mode)
        {
            state->SetMovementModeToggled(!new_combat_state);
        }
        if (new_combat_state == false && *settings->walk_after_combat)
        {
            state->walking_toggled = true;
        }
        state->old_combat_state = new_combat_state;
        state->combat_state_initiliazed = true;
        state->last_time_combat_state_changed = SDL_GetTicks();
    }

    return OriginalFunc(a1, a2, a3, a4);
}
