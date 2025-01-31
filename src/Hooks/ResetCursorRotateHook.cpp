#include "ResetCursorRotateHook.hpp"
#include "Settings.hpp"
#include "State.hpp"

void ResetCursorRotateHook::EnableSpecifically(uintptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

// Called in GameThread, when Camera Rotate key is up or forced by game, e.g. dialog or UI.
void ResetCursorRotateHook::OverrideFunc(int64_t a1, int a2)
{
    if (*Settings::GetSingleton()->enable_improved_mouselook)
    {
        auto* state = State::GetSingleton();
        if (!state->set_is_rotating_was_faked)
        {
            state->mouselook_toggled = false;
        }
        state->set_is_rotating_was_faked = false;

        //  Skip on game launch / first load.
        if (state->cursor_position_to_restore.x != -1)
        {
            state->SetInternalIsRotating(false);
            state->HideCursor(false);
        }
    }

    return OriginalFunc(a1, a2);
}
