#include "PollEventHook.hpp"
#include "../InputFaker.hpp"
#include "../Settings.hpp"
#include "../State.hpp"
#include "SetVirtualCursorPosHook.hpp"

bool PollEventHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "E8 ?? ?? ?? ?? 85 ?? 0F ?? ?? ?? ?? ?? 0F ?? ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? F3 ?? ?? ?? ?? "
        "?? ?? ?? 0F ?? ?? ?? ?? ?? ?? ?? F3 ?? ?? ?? ?? ?? ?? ?? 4C ?? ?? 48 ?? ?? ?? 48 ?? ?? E8 "
        "?? ?? ?? ?? 84">()) };
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
        OriginalFunc = dku::Hook::write_call<5>(address, OverrideFunc);
        INFO("Hooked PollEventHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

// Called every frame?
int64_t PollEventHook::OverrideFunc(int64_t a1)
{
    auto* state = State::GetSingleton();

    if (*Settings::GetSingleton()->enable_improved_mouselook)
    {
        if (state->IsRotating() && *Settings::GetSingleton()->enable_rightclick_mouselook_fix &&
            state->rotate_start_time != 0)
        {
            InputFaker::SendMouseMotion(0, 0);
        }

        if (!state->IsRotating())
        {
            if (state->frames_to_restore_cursor_pos > 0)
            {
                POINT p = state->cursor_position_to_restore;
                --state->frames_to_restore_cursor_pos;
                if (state->frames_to_restore_cursor_pos == 0)
                {
                    state->cursor_position_to_restore.x = -1;
                }
                SDL_WarpMouseInWindow(state->sdl_window, (int)p.x, (int)p.y);
            }
        }
    }

    return OriginalFunc(a1);
}
