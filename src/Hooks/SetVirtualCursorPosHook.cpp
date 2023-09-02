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
        INFO("Hooked SetVirtualCursorPosHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

void SetVirtualCursorPosHook::HideCursor(QWORD* xy)
{
    auto state = State::GetSingleton();
    SDL_SetRelativeMouseMode(SDL_TRUE);
    Vector2* xy_v = reinterpret_cast<Vector2*>(xy);
    int w = 0;
    int h = -1000000;
    SDL_GetWindowSize(state->sdl_window, &w,
        NULL);  // TODO can be removed if I manage to block interact
    // Centering x is not necessary, but may be useful at some point.
    xy_v->x = w / 2;
    xy_v->y = h;
    state->rotate_start_time = 0;
}

// Called every frame that has a mouse motion event
void SetVirtualCursorPosHook::OverrideFunc(QWORD* a1, QWORD* xy)
{
    auto state = State::GetSingleton();
    auto settings = Settings::GetSingleton();
    if (state->IsRotating())
    {
        if (state->is_rotating_changed)
        {
            state->is_rotating_changed = false;
            if (!*settings->enable_rightclick_mouselook_fix)
            {
                HideCursor(xy);
                return OriginalFunc(a1, xy);
            }
        }
        if (*settings->enable_rightclick_mouselook_fix && state->rotate_start_time != 0 &&
            SDL_GetTicks() - state->rotate_start_time >
                *settings->rightclick_threshold)
        {
            HideCursor(xy);
            return OriginalFunc(a1, xy);
        }
        return;
    }
    else
    {
        if (state->is_rotating_changed)
        {
            state->is_rotating_changed = false;
            state->rotate_start_time = 0;
            SDL_SetRelativeMouseMode(SDL_FALSE);
        }
        if (state->frames_to_restore_cursor_pos > 0)
        {
            return;
        }
    }

    return OriginalFunc(a1, xy);
}
