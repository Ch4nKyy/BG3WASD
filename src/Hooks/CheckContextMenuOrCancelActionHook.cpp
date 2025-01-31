#include "CheckContextMenuOrCancelActionHook.hpp"
#include "GameCommand.hpp"
#include "Settings.hpp"
#include "State.hpp"

using enum GameCommand;

void CheckContextMenuOrCancelActionHook::EnableSpecifically(uintptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

// If this function returns 0 for the CancelAction command, the command is forwarded to
// the MoveInputHandler.
// If it is 1, then it is not forwarded, but then it blocks the Rotate command as well.
// Interesting is, if I manipulate the command input struct here, it is modified in the
// MoveInputHandler as well!
int64_t CheckContextMenuOrCancelActionHook::OverrideFunc(int64_t a1, int64_t a2,
    int* SomeInputStruct, int16_t a4, int64_t a5)
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    int command_id = *SomeInputStruct;
    if (command_id != ActionCancel && command_id != ContextMenu)
    {
        return OriginalFunc(a1, a2, SomeInputStruct, a4, a5);
    }

    if (command_id == ActionCancel)
    {
        if (state->is_force_stop)
        {
            state->is_force_stop = false;
            return OriginalFunc(a1, a2, SomeInputStruct, a4, a5);
        }
    }

    if (*settings->enable_improved_mouselook)
    {
        if (command_id == ActionCancel)
        {
            bool is_key_down = *(reinterpret_cast<bool*>(SomeInputStruct) + 28);
            if (is_key_down)
            {
                *(int*)(SomeInputStruct) = 0;
                state->last_time_cancel_action_pressed = SDL_GetTicks();
            }
            else
            {
                uint32_t time_now = SDL_GetTicks();
                uint32_t time_diff_millis = time_now - state->last_time_cancel_action_pressed;
                if (time_diff_millis > *Settings::GetSingleton()->rotate_threshold)
                {
                    *(int*)(SomeInputStruct) = 0;
                }
            }
        }
        if (command_id == ContextMenu)
        {
            bool is_key_down = *(reinterpret_cast<bool*>(SomeInputStruct) + 28);
            if (is_key_down)
            {
                *(int*)(SomeInputStruct) = 0;
                state->last_time_context_menu_pressed = SDL_GetTicks();
            }
            else
            {
                uint32_t time_now = SDL_GetTicks();
                uint32_t time_diff_millis = time_now - state->last_time_context_menu_pressed;
                if (time_diff_millis > *Settings::GetSingleton()->rotate_threshold)
                {
                    *(int*)(SomeInputStruct) = 0;
                }
            }
        }
    }

    return OriginalFunc(a1, a2, SomeInputStruct, a4, a5);
}
