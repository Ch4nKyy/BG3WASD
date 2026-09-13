#include "MenuOpenedCavehook.hpp"
#include "Settings.hpp"
#include "State.hpp"

void MenuOpenedCavehook::EnableSpecifically(intptr_t address_incl_offset)
{
    auto [prolog, epilog] = dku::Hook::JIT::MakeNonVolatilePatch({ dku::Hook::Register::ALL });
    auto handle = DKUtil::Hook::AddCaveHook(address_incl_offset, { 0, 6 }, FUNC_INFO(Func), &prolog,
        &epilog, DKUtil::Hook::HookFlag::kRestoreAfterEpilog);
    handle->Enable();
    handles.emplace_back(std::move(handle));
}

void MenuOpenedCavehook::Func()
{
    auto* state = State::GetSingleton();

    state->is_menu_open = true;
    state->is_camera_rotation_toggle_dirty = true;
}
