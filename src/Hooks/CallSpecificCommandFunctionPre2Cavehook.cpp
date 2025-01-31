#include "CallSpecificCommandFunctionPre2Cavehook.hpp"
#include "GameCommand.hpp"
#include "InputFaker.hpp"
#include "Hooks/BlockCancelActionStoppingMovementPatch.hpp"
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

void CallSpecificCommandFunctionPre2Cavehook::EnableSpecifically(uintptr_t address_incl_offset)
{
    CallSpecificCommandFunctionPre2Prolog prolog;
    prolog.ready();
    CallSpecificCommandFunctionPre2Epilog epilog;
    epilog.ready();
    auto handle = DKUtil::Hook::AddCaveHook(address_incl_offset, { 0, 6 }, FUNC_INFO(Func), &prolog,
        &epilog, DKUtil::Hook::HookFlag::kRestoreAfterEpilog);
    handle->Enable();
    handles.emplace_back(std::move(handle));
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
