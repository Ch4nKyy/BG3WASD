#include "PollEventHook.hpp"
#include "InputFaker.hpp"
#include "Settings.hpp"
#include "State.hpp"
#include "SetVirtualCursorPosHook.hpp"

void PollEventHook::EnableSpecifically(uintptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<6>(address_incl_offset, OverrideFunc);
}

// Actual hiding happens in SetVirtualCursorPosHook.
// Sending a fake mouse input guarantees that it is called.
void PollEventHook::HideVirtualCursor(bool in_value)
{
    auto* state = State::GetSingleton();
    state->should_hide_virtual_cursor = in_value;
    state->rotate_start_time = 0;
    InputFaker::SendMouseMotion(0, 0);
}

// Called in MainThread, every frame
// TODO the camera has a flag "mouserotation". Maybe it is simpler to use than what we do right now?
int64_t PollEventHook::OverrideFunc(int64_t a1)
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    if (*settings->enable_improved_mouselook)
    {
        const std::lock_guard<std::mutex> lock(state->hide_cursor_mutex);
        if (state->ShouldHideCursor())
        {
            if (!state->cursor_hidden_last_frame)
            {
                SDL_SetRelativeMouseMode(SDL_TRUE);
                state->rotate_start_time = SDL_GetTicks();
            }
            if (state->rotate_start_time != 0 && SDL_GetTicks() - state->rotate_start_time >
                                                     *Settings::GetSingleton()->rotate_threshold)
            {
                HideVirtualCursor(true);
            }
        }
        else
        {
            if (state->cursor_hidden_last_frame)
            {
                POINT p = state->cursor_position_to_restore;
                SDL_WarpMouseInWindow(state->sdl_window, (int)p.x, (int)p.y);
                SDL_SetRelativeMouseMode(SDL_FALSE);
                HideVirtualCursor(false);
            }
        }
        state->cursor_hidden_last_frame = state->ShouldHideCursor();
    }

    return OriginalFunc(a1);
}
