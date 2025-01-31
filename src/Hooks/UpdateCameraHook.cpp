#include "UpdateCameraHook.hpp"
#include "Hooks/IsInControllerModeHook.hpp"
#include "Settings.hpp"
#include "State.hpp"

void UpdateCameraHook::EnableSpecifically(uintptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

// Called in GameThread, every frame? Called before HandleCameraInput.
int64_t UpdateCameraHook::OverrideFunc(uint64_t a1, uint64_t a2, uint64_t a3, int64_t a4)
{
    auto* settings = Settings::GetSingleton();
    auto* state = State::GetSingleton();

    if (IsInControllerModeHook::Get().Read())
    {
        return OriginalFunc(a1, a2, a3, a4);
    }

    int64_t camera_object_ptr = *(int64_t*)(a4 + 48);

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
