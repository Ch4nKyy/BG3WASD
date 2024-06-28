#include "PollEventHook.hpp"
#include "InputFaker.hpp"
#include "Settings.hpp"
#include "State.hpp"
#include "SetVirtualCursorPosHook.hpp"

bool PollEventHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "FF ?? ?? ?? ?? ?? 85 ?? 0F ?? ?? ?? ?? ?? 8B ?? ?? ?? 49 ?? ?? ?? 0F">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("PollEventHook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void PollEventHook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<6>(address, OverrideFunc);
        DEBUG("Hooked PollEventHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
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
