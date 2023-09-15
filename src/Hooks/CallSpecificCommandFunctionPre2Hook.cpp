#include "CallSpecificCommandFunctionPre2Hook.hpp"
#include "../GameCommand.hpp"
#include "../InputFaker.hpp"
#include "../Patches/BlockCancelActionStoppingMovementPatch.hpp"
#include "../Settings.hpp"
#include "../State.hpp"

using enum GameCommand;

bool CallSpecificCommandFunctionPre2Hook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "48 ?? ?? ?? ?? ?? ?? 4C ?? ?? 48 ?? ?? E8 ?? ?? ?? ?? 48 ?? ?? ?? ?? 48 ?? ?? 48 ?? ?? ?? "
        "?? 48 ?? ?? ?? ?? 48">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("CallSpecificCommandFunctionPre2Hook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void CallSpecificCommandFunctionPre2Hook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        auto offset = 13;
        OriginalFunc = dku::Hook::write_call<5>(address + offset, OverrideFunc);
        DEBUG("Hooked CallSpecificCommandFunctionPre2Hook #{}: {:X}", i,
            AsAddress(address + offset));
        ++i;
    }
}

// GameThread
// This hook is awesome.
// We can use it to detect input commands and react to them.
// But we can also use the original function to send input commands directly instead of sending
// keys.
WORD* CallSpecificCommandFunctionPre2Hook::OverrideFunc(int64_t* a1, WORD* a2, int* command_struct)
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    if (a1 && !InputFaker::game_input_manager)
    {
        InputFaker::game_input_manager = a1;
    }

    int command_id = *(int*)command_struct;
    bool is_key_down = *(reinterpret_cast<bool*>(command_struct) + 28);
    if (state->IsCurrentlyInteractMoving() && state->IsCharacterMovementMode() &&
        command_id >= 142 && command_id <= 145 && is_key_down)
    {
        state->force_stop = true;
        InputFaker::SendCommand(ActionCancel, SDL_RELEASED);
    }

    return OriginalFunc(a1, a2, command_struct);
}
