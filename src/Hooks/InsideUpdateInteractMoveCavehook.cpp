#include "InsideUpdateInteractMoveCavehook.hpp"
#include "Settings.hpp"
#include "State.hpp"

void InsideUpdateInteractMoveCavehook::EnableSpecifically(intptr_t address_incl_offset)
{
    auto [prolog, epilog] = dku::Hook::JIT::MakeNonVolatilePatch({ dku::Hook::Register::ALL });
    auto handle = DKUtil::Hook::AddCaveHook(address_incl_offset, { 0, 7 }, FUNC_INFO(Func), &prolog,
        &epilog, DKUtil::Hook::HookFlag::kRestoreBeforeProlog);
    handle->Enable();
    handles.emplace_back(std::move(handle));
}

void InsideUpdateInteractMoveCavehook::Func()
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    state->SetCurrentlyInteractMoving(true);
}
