#include "SetVirtualCursorPosHook.hpp"
#include "Settings.hpp"
#include "State.hpp"
#include "Structs/SetVirtualCursorPosFakeClass.hpp"
#include "Structs/Vector2.hpp"
#include "SDL.h"

bool SetVirtualCursorPosHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(
        dku::Hook::Assembly::search_pattern<"F3 0F 11 4D ?? FF ?? 08 01 00 00 ?? ?? ?? ?? 48">()) };
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
        // Since this is a vfunction call, the returned address of the original func will be wrong,
        // so don't store it.
        dku::Hook::write_call<6>(address + 5, OverrideFunc);
        DEBUG("Hooked SetVirtualCursorPosHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

// Called in MainThread, every frame that has a mouse motion event
QWORD* SetVirtualCursorPosHook::OverrideFunc(int64_t* self, char* a2, int* a3)
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    SetVirtualCursorPosFakeClass* self_fake = reinterpret_cast<SetVirtualCursorPosFakeClass*>(self);

    auto* xy = self_fake->SetVirtualCursorPos(a2, a3);

    if (*settings->enable_improved_mouselook)
    {
        if (state->should_hide_virtual_cursor)
        {
            Vector2* xy_v = reinterpret_cast<Vector2*>(xy);
            int w = 0;
            int h = -1000000;
            SDL_GetWindowSize(state->sdl_window, &w,
                NULL);
            // Centering x is not necessary, but may be useful at some point.
            xy_v->x = w / 2;
            xy_v->y = h;
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
        }
        state->virtual_cursor_hidden_last_frame = state->should_hide_virtual_cursor;
    }

    return xy;
}
