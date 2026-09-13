#include "PollEventHook.hpp"
#include "InputFaker.hpp"
#include "RotationCursorType.hpp"
#include "SetVirtualCursorPosHook.hpp"
#include "Settings.hpp"
#include "State.hpp"

void PollEventHook::EnableSpecifically(intptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<6>(address_incl_offset, OverrideFunc);
}

int32_t PollEventHook::DistanceSquaredBetweenPoints(POINT a, POINT b)
{
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

int64_t PollEventHook::OverrideFunc(int64_t a1)
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    const std::lock_guard<std::mutex> lock(state->override_cursor_mutex);
    if (state->ShouldOverrideCursor())
    {
        if (!state->cursor_overriden_last_frame)
        {
            SDL_SetRelativeMouseMode(SDL_TRUE);
            state->virtual_cursor_state = VirtualCursorState::REQUEST_OVERRIDE;
        }

        constexpr int DISTANCE_THRESHOLD_SQUARED = 6 * 6;
        if (state->virtual_cursor_state == VirtualCursorState::REQUEST_OVERRIDE &&
            DistanceSquaredBetweenPoints(state->cursor_position_to_restore,
                state->virtual_cursor_position) > DISTANCE_THRESHOLD_SQUARED)
        {
            state->virtual_cursor_state = VirtualCursorState::OVERRIDE;
            InputFaker::SendMouseMotion(0, 0);  // force cursor update
        }
    }
    else
    {
        if (state->cursor_overriden_last_frame)
        {
            POINT p = state->cursor_position_to_restore;
            SDL_WarpMouseInWindow(state->sdl_window, (int)p.x, (int)p.y);
            SDL_SetRelativeMouseMode(SDL_FALSE);

            state->virtual_cursor_state = VirtualCursorState::END_OVERRIDE;
            InputFaker::SendMouseMotion(0, 0);  // force cursor update
        }
    }
    state->cursor_overriden_last_frame = state->ShouldOverrideCursor();

    return OriginalFunc(a1);
}
