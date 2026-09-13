#include "UpdateCameraHook.hpp"
#include "Hooks/IsInControllerModeGlobalRef.hpp"
#include "Settings.hpp"
#include "State.hpp"
#include "Structs/Camera.hpp"

void UpdateCameraHook::EnableSpecifically(intptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

int64_t UpdateCameraHook::OverrideFunc(uint64_t a1, uint64_t a2, uint64_t a3, int64_t a4)
{
    if (IsInControllerModeGlobalRef::Get().Read())
    {
        return OriginalFunc(a1, a2, a3, a4);
    }

    auto* settings = Settings::GetSingleton();
    auto* state = State::GetSingleton();

    auto* camera = *reinterpret_cast<Camera**>(a4 + 48);
    state->SetCamera(camera);

    bool new_is_in_combat = camera->cameraModeFlags & CameraModeFlags::kCombat;
    if (!state->combat_state_initialized || new_is_in_combat != state->is_in_combat)
    {
        if (*settings->auto_toggle_movement_mode)
        {
            state->SetMovementModeToggled(!new_is_in_combat);
        }
        if (new_is_in_combat == false && *settings->walk_after_combat)
        {
            state->walking_toggled = true;
        }
        state->is_in_combat = new_is_in_combat;
        state->combat_state_initialized = true;
        state->last_time_combat_state_changed = SDL_GetTicks();
    }

    return OriginalFunc(a1, a2, a3, a4);
}
