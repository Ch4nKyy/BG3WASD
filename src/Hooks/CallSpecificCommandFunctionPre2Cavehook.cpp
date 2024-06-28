#include "CallSpecificCommandFunctionPre2Cavehook.hpp"
#include "GameCommand.hpp"
#include "InputFaker.hpp"
#include "Patches/BlockCancelActionStoppingMovementPatch.hpp"
#include "Settings.hpp"
#include "State.hpp"

using enum GameCommand;

struct CallSpecificCommandFunctionPre2Prolog : Xbyak::CodeGenerator
{
    CallSpecificCommandFunctionPre2Prolog()
    {
        push(rax);
        push(rbx);
        push(rcx);
        push(rdx);
        push(r8);
    }
};

struct CallSpecificCommandFunctionPre2Epilog : Xbyak::CodeGenerator
{
    CallSpecificCommandFunctionPre2Epilog()
    {
        pop(r8);
        pop(rdx);
        pop(rcx);
        pop(rbx);
        pop(rax);
    }
};

bool CallSpecificCommandFunctionPre2Cavehook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(
        dku::Hook::Assembly::search_pattern<"FF 50 68 48 8B C3">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("CallSpecificCommandFunctionPre2Cavehook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void CallSpecificCommandFunctionPre2Cavehook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        CallSpecificCommandFunctionPre2Prolog prolog;
        prolog.ready();
        CallSpecificCommandFunctionPre2Epilog epilog;
        epilog.ready();
        handle = DKUtil::Hook::AddCaveHook(address, { 0, 6 }, FUNC_INFO(Func), &prolog, &epilog,
            DKUtil::Hook::HookFlag::kRestoreAfterEpilog);
        handle->Enable();
        DEBUG("Hooked CallSpecificCommandFunctionPre2Cavehook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

void CallSpecificCommandFunctionPre2Cavehook::Func(int64_t* self, WORD* a2, int* command_struct)
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    InputFaker::game_input_manager = *(int64_t**)self;

    int command_id = *(int*)command_struct;
    bool is_key_down = *(reinterpret_cast<bool*>(command_struct) + 28);
    if (state->IsCurrentlyInteractMoving() && state->IsCharacterMovementMode() &&
        command_id >= CharacterMoveForward && command_id <= CharacterMoveRight && is_key_down)
    {
        // Calling InputFaker::SendCommand here crashes since CallSpecificCommandFunctionPre2 is a
        // virtual call and I don't fully understand why.
        // The easy workaround is to set the order_force_stop flag here and call it somewhere else.
        // Inside CheckCommandInputsHook, which is called EVERY frame, this flag is checked
        // and then reacted accordingly.
        state->order_force_stop = true;
    }
}
