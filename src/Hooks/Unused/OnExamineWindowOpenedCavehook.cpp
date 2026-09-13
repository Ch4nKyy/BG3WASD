#include "OnExamineWindowOpenedCavehook.hpp"
#include "State.hpp"

void OnExamineWindowOpenedCavehook::EnableSpecifically(intptr_t address_incl_offset)
{
    auto [prolog, epilog] = dku::Hook::JIT::MakeNonVolatilePatch({ dku::Hook::Register::ALL });
    auto handle = DKUtil::Hook::AddCaveHook(address_incl_offset, { 0, 7 }, FUNC_INFO(Func), &prolog,
        &epilog, DKUtil::Hook::HookFlag::kRestoreAfterEpilog);
    handle->Enable();
    handles.emplace_back(std::move(handle));
}

void OnExamineWindowOpenedCavehook::Func()
{
    auto* state = State::GetSingleton();
}
