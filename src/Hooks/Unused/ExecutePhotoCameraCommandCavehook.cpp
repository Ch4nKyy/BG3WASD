#include "ExecutePhotoCameraCommandCavehook.hpp"
#include "State.hpp"

struct MoveRBXToR8 : Xbyak::CodeGenerator
{
    MoveRBXToR8() { mov(r8, rbx); }
};

void ExecutePhotoCameraCommandCavehook::EnableSpecifically(intptr_t address_incl_offset)
{
    auto [prolog, epilog] = dku::Hook::JIT::MakeNonVolatilePatch({ dku::Hook::Register::ALL });
    MoveRBXToR8 patch;
    patch.ready();
    prolog.Append(patch);
    auto handle = DKUtil::Hook::AddCaveHook(address_incl_offset, { 0, 5 }, FUNC_INFO(Func), &prolog,
        &epilog, DKUtil::Hook::HookFlag::kRestoreAfterEpilog);
    handle->Enable();
    handles.emplace_back(std::move(handle));
}

void ExecutePhotoCameraCommandCavehook::Func(int64_t* camera_manager, int64_t a2, Command* command)
{
    auto state = State::GetSingleton();
}
