#include "BlockCameraInputCavehook.hpp"
#include "Hooks/IsInControllerModeHook.hpp"
#include "Settings.hpp"
#include "State.hpp"

struct BlockCameraInputProlog : Xbyak::CodeGenerator
{
    BlockCameraInputProlog()
    {
        push(rax);
        push(rbx);
        push(rcx);
        push(rdx);
    }
};

struct BlockCameraInputEpilog : Xbyak::CodeGenerator
{
    BlockCameraInputEpilog()
    {
        mov(r12b, al);
        mov(ptr[rdi + 324], r12b);
        pop(rdx);
        pop(rcx);
        pop(rbx);
        pop(rax);
    }
};

void BlockCameraInputCavehook::EnableSpecifically(uintptr_t address_incl_offset)
{
    BlockCameraInputProlog prolog;
    prolog.ready();
    BlockCameraInputEpilog epilog;
    epilog.ready();
    auto handle = DKUtil::Hook::AddCaveHook(address_incl_offset, { 0, 7 }, FUNC_INFO(Func), &prolog, &epilog,
        DKUtil::Hook::HookFlag::kSkipNOP);
    handle->Enable();
    handles.emplace_back(std::move(handle));
}

// Write 1 or 0 into rax. Then, write al into r12b. Then, write r12b into [rdi+324], which is the
// original behaviour of the hooked line. rdi is the camera object. 324 is "should_move".
//
// In the past, we also set the movement input to 0 (cam + 148 and cam + 152). This blocked
// camera movement in combat, when character movement mode was active. But not blocking it, is
// actually better. And we don't need to get the camera object.
bool BlockCameraInputCavehook::Func()
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    if (IsInControllerModeHook::Get().Read())
    {
        return 1;
    }

    return !state->IsCharacterMovementMode();
}
