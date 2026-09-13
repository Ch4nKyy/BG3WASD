#include "TickInputSystemHook.hpp"
#include "CommandId.hpp"
#include "InputFaker.hpp"
#include "Settings.hpp"
#include "State.hpp"

using enum CommandId;

void TickInputSystemHook::EnableSpecifically(intptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

char TickInputSystemHook::OverrideFunc(int64_t a1, float* a2)
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    state->UpdateBlockCancelActionStoppingMovementPatch();
    InputFaker::ProcessQueuedCommands();

    if (state->is_camera_rotation_toggle_dirty || state->is_camera_rotation_toggle_very_dirty)
    {
        if (state->IsCameraRotationToggled())
        {
            if (!state->IsRotating() || state->is_camera_rotation_toggle_very_dirty)
            {
                state->EnableCameraRotation(false);  // needed! otherwise can get stuck.
                state->EnableCameraRotation(true);
            }
        }
        else
        {
            // This logic is not perfect.
            // If you hold to rotate, with toggle off, and then the toggle flag gets dirty, camera rotation will stop.
            // Maybe we need to detect inside CameraRotationOn/Off whether it's real or fake input.
            // But CameraRotationOff is sometimes called twice or thrice.
            if (state->IsRotating() || state->is_camera_rotation_toggle_very_dirty)
            {
                state->EnableCameraRotation(false);
            }
        }

        state->is_camera_rotation_toggle_dirty = false;
        state->is_camera_rotation_toggle_very_dirty = false;
    }

    return OriginalFunc(a1, a2);
}
