#include "PhotomodeClosedCavehook.hpp"
#include "State.hpp"

void PhotomodeClosedCavehook::EnableSpecifically(intptr_t address_incl_offset)
{
    auto [prolog, epilog] = dku::Hook::JIT::MakeNonVolatilePatch({ dku::Hook::Register::ALL });
    auto handle = DKUtil::Hook::AddCaveHook(address_incl_offset, { 0, 7 }, FUNC_INFO(Func), &prolog,
        &epilog, DKUtil::Hook::HookFlag::kRestoreAfterEpilog);
    handle->Enable();
    handles.emplace_back(std::move(handle));
}

void PhotomodeClosedCavehook::Func()
{
    auto state = State::GetSingleton();

    state->is_in_photomode = false;
    state->is_camera_rotation_toggle_dirty = true;
}
