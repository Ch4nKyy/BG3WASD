#include "SetVirtualCursorPosHook.hpp"
#include "../State.hpp"
#include "../Structs/Vector2.hpp"
#include "SDL.h"


bool SetVirtualCursorPosHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "E8 ?? ?? ?? ?? BA ?? ?? ?? ?? C7 ?? ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? C7 ?? ?? ?? ?? ?? ?? ?? "
        "44 ?? ?? ?? ?? 44 ?? ?? ?? E8 ?? ?? ?? ?? 4C ?? ?? ?? ?? 48 ?? ?? 48 ?? ?? ?? E8 ?? ?? ?? "
        "?? 48 ?? ?? 45 ?? ?? ?? 49 ?? ?? E8 ?? ?? ?? ?? 44 ?? ?? 44 ?? ?? E9 ?? ?? ?? ?? 2D">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("SetVirtualCursorPosHook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void SetVirtualCursorPosHook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<5>(address, OverrideFunc);
        INFO("Hooked SetVirtualCursorPosHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

void SetVirtualCursorPosHook::OverrideFunc(QWORD* a1, QWORD* xy)
{
    auto state = State::GetSingleton();
    if (state->is_rotating)
    {
        if (state->is_rotating_changed)
        {
            state->is_rotating_changed = false;
            SDL_SetRelativeMouseMode(SDL_TRUE);
            Vector2* xy_v = reinterpret_cast<Vector2*>(xy);
            int w = 0;
            int h = -1000000;
            SDL_GetWindowSize(state->sdl_window, &w,
                NULL);  // TODO can be removed if I manage to block interact
            xy_v->x = w / 2;
            xy_v->y = h;
            return OriginalFunc(a1, xy);
        }
        return;
    }
    else
    {
        if (state->is_rotating_changed)
        {
            state->is_rotating_changed = false;
            SDL_SetRelativeMouseMode(SDL_FALSE);
        }
        // If we only set the pos once there is some kind of race condition.
        if (state->frames_to_restore_cursor_pos > 0)
        {
            POINT p = state->cursor_position_to_restore;
            --state->frames_to_restore_cursor_pos;
            SDL_WarpMouseInWindow(state->sdl_window, (int)p.x, (int)p.y);
            return;
        }
    }

    return OriginalFunc(a1, xy);
}
