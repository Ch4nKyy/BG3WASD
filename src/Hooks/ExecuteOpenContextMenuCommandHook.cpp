#include "ExecuteOpenContextMenuCommandHook.hpp"
#include "CommandId.hpp"
#include "Settings.hpp"
#include "State.hpp"
#include "Structs/Command.hpp"

using enum CommandId;

void ExecuteOpenContextMenuCommandHook::EnableSpecifically(intptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

int64_t ExecuteOpenContextMenuCommandHook::OverrideFunc(int64_t a1, int64_t a2, Command* command,
    int16_t a4, int64_t a5)
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    if (command->id == ActionCancel)
    {
        // This block is for the feature "pressing any movement key while interact move mode is active, will cancel
        // interact move mode". It does this by sending an artificial ActionCancel command. This command should not get
        // the special treatment of the block below.
        if (state->is_force_stop)
        {
            state->is_force_stop = false;
            return OriginalFunc(a1, a2, command, a4, a5);
        }

        if (command->new_pressed)
        {
            command->id = 0;
        }
        else
        {
            if (state->virtual_cursor_state == VirtualCursorState::OVERRIDE)
            {
                command->id = 0;
            }
        }
    }
    else if (command->id == ContextMenu)
    {
        if (command->new_pressed)
        {
            command->id = 0;
        }
        else
        {
            if (state->virtual_cursor_state == VirtualCursorState::OVERRIDE)
            {
                command->id = 0;
            }
        }
    }

    return OriginalFunc(a1, a2, command, a4, a5);
}
