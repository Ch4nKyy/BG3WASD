#include "CastOrCancelAbilityHook.hpp"
#include "GameCommand.hpp"
#include "Settings.hpp"
#include "State.hpp"

using enum GameCommand;

void CastOrCancelAbilityHook::EnableSpecifically(uintptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

WORD* CastOrCancelAbilityHook::OverrideFunc(WORD* a1, int* command_struct, uint64_t* a3, int64_t a4,
    int64_t a5, int64_t a6, int64_t a7, int64_t a8, int64_t a9, int64_t a10, int64_t a11,
    int64_t a12, int64_t a13, int64_t a14, int64_t a15, int64_t a16)
{
    auto* state = State::GetSingleton();
    int command_id = *command_struct;
    if (command_id != ActionCancel)
    {
        return OriginalFunc(a1, command_struct, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14,
            a15, a16);
    }

    if (*Settings::GetSingleton()->enable_improved_mouselook)
    {
        if (command_id == ActionCancel)
        {
            bool is_key_down = *(reinterpret_cast<bool*>(command_struct) + 28);
            if (is_key_down)
            {
                *(WORD*)a1 = 0;
                return a1;
            }
            else
            {
                uint32_t time_now = SDL_GetTicks();
                uint32_t time_diff_millis = time_now - state->last_time_cancel_action_pressed;
                if (time_diff_millis > *Settings::GetSingleton()->rotate_threshold)
                {
                    *(WORD*)a1 = 0;
                    return a1;
                }
            }
        }
    }

    return OriginalFunc(a1, command_struct, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14,
        a15, a16);
}
