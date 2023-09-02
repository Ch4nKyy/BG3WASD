#include "CheckCommandInputsHook.hpp"
#include "../InputFaker.hpp"
#include "../Settings.hpp"
#include "../State.hpp"
#include "SetVirtualCursorPosHook.hpp"

bool CheckCommandInputsHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(
        dku::Hook::Assembly::search_pattern<"E8 ?? ?? ?? ?? 48 ?? ?? ?? E8 ?? ?? ?? ?? 48 ?? ?? ?? "
                                            "?? ?? ?? 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? FF">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("CheckCommandInputsHook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void CheckCommandInputsHook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<5>(address, OverrideFunc);
        INFO("Hooked CheckCommandInputsHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

// Called in GameThread, every frame
char CheckCommandInputsHook::OverrideFunc(int64_t a1, float* a2)
{
    auto* state = State::GetSingleton();

    if (*Settings::GetSingleton()->enable_improved_mouselook)
    {
        if (state->IsRotating() && *Settings::GetSingleton()->enable_rightclick_mouselook_fix &&
            state->rotate_start_time != 0)
        {
            InputFaker::SendMouseMotion(0, 0);
        }

        if (state->player_can_input_movement)
        {
            switch (state->mouselook_request)
            {
            case ToggleRequest::OFF:
                state->SetIsRotating(false);
                state->mouselook_toggled = false;
                state->mouselook_request = ToggleRequest::NONE;
                break;
            case ToggleRequest::ON:
                state->SetIsRotating(true);
                state->mouselook_toggled = true;
                state->mouselook_request = ToggleRequest::NONE;
                break;
            case ToggleRequest::ON_AND_OFF:  // needed when tabbing in
                state->SetIsRotating(true);
                state->SetIsRotating(false);
                state->mouselook_toggled = false;
                state->mouselook_request = ToggleRequest::NONE;
                break;
            case ToggleRequest::NONE:
            default:
                break;
            }
        }

        if (state->player_could_input_movement_last_frame != state->player_can_input_movement)
        {
            if (state->player_can_input_movement && state->mouselook_toggled)
            {
                state->SetIsRotating(true);
            }
            if (!state->player_can_input_movement && state->IsRotating())
            {
                state->SetIsRotating(false);
            }
        }
        state->player_could_input_movement_last_frame = state->player_can_input_movement;
        state->player_can_input_movement = false;

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

    return OriginalFunc(a1, a2);
}
