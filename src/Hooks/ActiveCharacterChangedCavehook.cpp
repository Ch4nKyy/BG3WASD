#include "ActiveCharacterChangedCavehook.hpp"
#include "State.hpp"

void ActiveCharacterChangedCavehook::EnableSpecifically(intptr_t address_incl_offset)
{
    auto [prolog, epilog] = dku::Hook::JIT::MakeNonVolatilePatch({ dku::Hook::Register::ALL });
    auto handle = DKUtil::Hook::AddCaveHook(address_incl_offset, { 0, 5 }, FUNC_INFO(Func), &prolog,
        &epilog, DKUtil::Hook::HookFlag::kRestoreAfterEpilog);
    handle->Enable();
    handles.emplace_back(std::move(handle));
}

void ActiveCharacterChangedCavehook::Func(int64_t invalid, int64_t invalid2, int64_t invalid3,
    int64_t character)
{
    auto state = State::GetSingleton();

    state->active_character = character;
    state->is_camera_rotation_toggle_very_dirty = true;
}
