#include "SetVirtualCursorPosHook.hpp"
#include "../Settings.hpp"
#include "../State.hpp"
#include "../Structs/Vector2.hpp"
#include "SDL.h"

bool SetVirtualCursorPosHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "E8 ?? ?? ?? ?? BA ?? ?? ?? ?? C7 ?? ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? C7">()) };
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
        DEBUG("Hooked SetVirtualCursorPosHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

// Called in MainThread, every frame that has a mouse motion event
void SetVirtualCursorPosHook::OverrideFunc(QWORD* a1, QWORD* xy)
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    if (!*settings->enable_improved_mouselook)
    {
        return OriginalFunc(a1, xy);
    }

    if (state->should_hide_virtual_cursor)
    {
        if (!state->virtual_cursor_hidden_last_frame)
        {
            Vector2* xy_v = reinterpret_cast<Vector2*>(xy);
            int w = 0;
            int h = -1000000;
            SDL_GetWindowSize(state->sdl_window, &w,
                NULL);  // TODO can be removed if I manage to block interact
            // Centering x is not necessary, but may be useful at some point.
            xy_v->x = w / 2;
            xy_v->y = h;
            OriginalFunc(a1, xy);
        }
        else
        {
            return;
        }
    }
    else
    {
        if (state->virtual_cursor_hidden_last_frame)
        {
            POINT p = state->cursor_position_to_restore;
            Vector2* xy_v = reinterpret_cast<Vector2*>(xy);
            xy_v->x = (int)p.x;
            xy_v->y = (int)p.y;
        }
        OriginalFunc(a1, xy);
    }
    state->virtual_cursor_hidden_last_frame = state->should_hide_virtual_cursor;

    return;
}
