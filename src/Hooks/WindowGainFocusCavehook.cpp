#include "WindowGainFocusCavehook.hpp"
#include "State.hpp"

void WindowGainFocusCavehook::EnableSpecifically(intptr_t address_incl_offset)
{
    auto [prolog, epilog] = dku::Hook::JIT::MakeNonVolatilePatch({ dku::Hook::Register::ALL });
    auto handle = DKUtil::Hook::AddCaveHook(address_incl_offset, { 0, 6 }, FUNC_INFO(Func), &prolog,
        &epilog, DKUtil::Hook::HookFlag::kRestoreAfterEpilog);
    handle->Enable();
    handles.emplace_back(std::move(handle));
}

void WindowGainFocusCavehook::Func()
{
    auto* state = State::GetSingleton();

    // When you tab out, camera rotation toggle gets stuck, so reset it on tabbing in.
    if (state->IsCameraRotationToggled())
    {
        state->is_camera_rotation_toggle_very_dirty = true;
    }
}
