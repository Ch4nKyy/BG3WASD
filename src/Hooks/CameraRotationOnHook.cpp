#include "CameraRotationOnHook.hpp"
#include "RotationCursorType.hpp"
#include "SDL.h"
#include "State.hpp"

void CameraRotationOnHook::EnableSpecifically(intptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

void CameraRotationOnHook::OverrideFunc(int64_t a1, int a2)
{
    auto* state = State::GetSingleton();

    bool was_already_rotating = state->IsRotating();

    state->SetInternalCameraRotationState(true);

    auto rotation_cursor_type = state->GetRotationCursorTypeInCurrentMode();
    switch (rotation_cursor_type)
    {
    case RotationCursorType::INVISIBLE:
        // SDL_HINT_MOUSE_RELATIVE_MODE_CENTER avoids a bug when spam-clicking Rotate where the cursor ends up in
        // weird positions.
        SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_CENTER, "0");
        SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_CURSOR_VISIBLE, "0");
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (!was_already_rotating && !state->is_camera_rotation_toggle_very_dirty)
        {
            state->cursor_position_to_restore.x = x;
            state->cursor_position_to_restore.y = y;
        }
        break;
    case RotationCursorType::CENTERED:
        SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_CENTER, "1");
        SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_CURSOR_VISIBLE, "1");
        if (!was_already_rotating && !state->is_camera_rotation_toggle_very_dirty)
        {
            int w, h;
            SDL_GetWindowSize(state->sdl_window, &w, &h);
            state->cursor_position_to_restore.x = w / 2;
            state->cursor_position_to_restore.y = h / 2;
        }
        break;
    }
    if (rotation_cursor_type != RotationCursorType::VANILLA)
    {
        state->OverrideCursor(true);
    }

    return OriginalFunc(a1, a2);
}
