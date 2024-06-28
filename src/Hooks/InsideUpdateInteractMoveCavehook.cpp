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

bool InsideUpdateInteractMoveCavehook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(
        dku::Hook::Assembly::search_pattern<"4C 8B 89 80 00 00 00 4C ?? ?? 30 01 00 00">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("InsideUpdateInteractMoveCavehook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void InsideUpdateInteractMoveCavehook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        InsideUpdateInteractMoveProlog prolog;
        prolog.ready();
        InsideUpdateInteractMoveEpilog epilog;
        epilog.ready();
        handle = DKUtil::Hook::AddCaveHook(address, { 0, 7 }, FUNC_INFO(Func), &prolog, &epilog,
            DKUtil::Hook::HookFlag::kRestoreBeforeProlog);
        handle->Enable();
        DEBUG("Hooked InsideUpdateInteractMoveCavehook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

void InsideUpdateInteractMoveCavehook::Func()
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    state->SetCurrentlyInteractMoving(true);
}
