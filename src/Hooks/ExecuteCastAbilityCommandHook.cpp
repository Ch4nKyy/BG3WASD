#include "ExecuteCastAbilityCommandHook.hpp"
#include "CommandId.hpp"
#include "Settings.hpp"
#include "State.hpp"
#include "Structs/Command.hpp"

using enum CommandId;

void ExecuteCastAbilityCommandHook::EnableSpecifically(intptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

WORD* ExecuteCastAbilityCommandHook::OverrideFunc(WORD* a1, Command* command, uint64_t* a3,
    int64_t a4, int64_t a5, int64_t a6, int64_t a7, int64_t a8, int64_t a9, int64_t a10,
    int64_t a11, int64_t a12, int64_t a13, int64_t a14, int64_t a15, int64_t a16)
{
    auto* state = State::GetSingleton();

    if (command->id == ActionCancel)
    {
        if (command->new_pressed)
        {
            *(WORD*)a1 = 0;
            return a1;
        }
        else
        {
            if (state->virtual_cursor_state == VirtualCursorState::OVERRIDE)
            {
                *(WORD*)a1 = 0;
                return a1;
            }
        }
    }

    return OriginalFunc(a1, command, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
}
