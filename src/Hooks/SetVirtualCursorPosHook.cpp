#include "SetVirtualCursorPosHook.hpp"
#include "RotationCursorType.hpp"
#include "SDL.h"
#include "Settings.hpp"
#include "State.hpp"
#include "Structs/SetVirtualCursorPosFakeClass.hpp"
#include "Structs/Vector2.hpp"

void SetVirtualCursorPosHook::EnableSpecifically(intptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<6>(address_incl_offset, OverrideFunc);
}

QWORD* SetVirtualCursorPosHook::OverrideFunc(int64_t* self, char* a2, int* a3)
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    SetVirtualCursorPosFakeClass* self_fake = reinterpret_cast<SetVirtualCursorPosFakeClass*>(self);

    auto* xy = self_fake->SetVirtualCursorPos(a2, a3);
    Vector2* xy_v = reinterpret_cast<Vector2*>(xy);

    state->virtual_cursor_position.x = xy_v->x;
    state->virtual_cursor_position.y = xy_v->y;

    switch (state->virtual_cursor_state)
    {
    case VirtualCursorState::OVERRIDE:
        {
            int w = 0, h = 0;
            SDL_GetWindowSize(state->sdl_window, &w, &h);

            auto rotation_cursor_type = state->GetRotationCursorTypeInCurrentMode();
            switch (rotation_cursor_type)
            {
            case RotationCursorType::INVISIBLE:
                xy_v->x = w / 2;
                xy_v->y = -1000000;
                break;
            case RotationCursorType::CENTERED:
                xy_v->x = w / 2;
                xy_v->y = h / 2;
                break;
            }
            break;
        }
    case VirtualCursorState::END_OVERRIDE:
        {
            POINT p = state->cursor_position_to_restore;
            Vector2* xy_v = reinterpret_cast<Vector2*>(xy);
            xy_v->x = (int)p.x;
            xy_v->y = (int)p.y;
            state->virtual_cursor_state = VirtualCursorState::NORMAL;
            break;
        }
    }

    return xy;
}
