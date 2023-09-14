#include "InsideHandleMoveInputHook.hpp"
#include "../InputFaker.hpp"
#include "../Patches/BlockCancelActionStoppingMovementPatch.hpp"
#include "../Settings.hpp"
#include "../State.hpp"

bool InsideHandleMoveInputHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "E8 ?? ?? ?? ?? 0F ?? ?? 66 ?? ?? ?? 84 ?? 74 ?? 48">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("InsideHandleMoveInputHook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void InsideHandleMoveInputHook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<5>(address, OverrideFunc);
        DEBUG("Hooked InsideHandleMoveInputHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

// GameThread
WORD* InsideHandleMoveInputHook::OverrideFunc(int64_t a1, WORD* a2, uint16_t* a3,
    int64_t command_struct)
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    int command_id = *(int*)command_struct;
    if (state->IsCurrentlyInteractMoving() && state->IsWasdCharacterMovement() &&
        command_id >= 142 && command_id <= 145)
    {
        InputFaker::SendKeyDownAndUp(state->cancel_keys[0]);
    }

    return OriginalFunc(a1, a2, a3, command_struct);
}
