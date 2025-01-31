#include "SetCursorRotateHook.hpp"
#include "Settings.hpp"
#include "State.hpp"

void SetCursorRotateHook::EnableSpecifically(uintptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

// Called in GameThread, when Camera Rotate down
void SetCursorRotateHook::OverrideFunc(int64_t a1, int a2)
{
    if (*Settings::GetSingleton()->enable_improved_mouselook)
    {
        auto* state = State::GetSingleton();

        state->SetInternalIsRotating(true);

        int x, y;
        SDL_GetMouseState(&x, &y);
        state->cursor_position_to_restore.x = x;
        state->cursor_position_to_restore.y = y;
        state->HideCursor(true);
    }

    return OriginalFunc(a1, a2);
}
