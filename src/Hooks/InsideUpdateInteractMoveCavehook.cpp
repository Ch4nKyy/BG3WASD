#include "InsideUpdateInteractMoveCavehook.hpp"
#include "Settings.hpp"
#include "State.hpp"

struct InsideUpdateInteractMoveProlog : Xbyak::CodeGenerator
{
    InsideUpdateInteractMoveProlog()
    {
        push(rax);
        push(rbx);
        push(rcx);
        push(rdx);
        push(r8);
        push(r9);
        push(r10);
        push(r11);
    }
};

struct InsideUpdateInteractMoveEpilog : Xbyak::CodeGenerator
{
    InsideUpdateInteractMoveEpilog()
    {
        pop(r11);
        pop(r10);
        pop(r9);
        pop(r8);
        pop(rdx);
        pop(rcx);
        pop(rbx);
        pop(rax);
    }
};

void InsideUpdateInteractMoveCavehook::EnableSpecifically(uintptr_t address_incl_offset)
{
    InsideUpdateInteractMoveProlog prolog;
        prolog.ready();
        InsideUpdateInteractMoveEpilog epilog;
        epilog.ready();
        auto handle = DKUtil::Hook::AddCaveHook(address_incl_offset, { 0, 7 }, FUNC_INFO(Func), &prolog, &epilog,
            DKUtil::Hook::HookFlag::kRestoreBeforeProlog);
        handle->Enable();
        handles.emplace_back(std::move(handle));
}

void InsideUpdateInteractMoveCavehook::Func()
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    state->SetCurrentlyInteractMoving(true);
}
