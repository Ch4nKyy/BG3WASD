#include "CameraRotationOffHook.hpp"
#include "RotationCursorType.hpp"
#include "State.hpp"

void CameraRotationOffHook::EnableSpecifically(intptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

void CameraRotationOffHook::OverrideFunc(int64_t a1, int a2)
{
    auto* state = State::GetSingleton();

    state->SetInternalCameraRotationState(false);

    //  Skip on game launch / first load.
    if (state->cursor_position_to_restore.x != -1)
    {
        state->is_camera_rotation_toggle_dirty = true;
        state->OverrideCursor(false);
    }

    return OriginalFunc(a1, a2);
}
