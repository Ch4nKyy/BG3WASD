#include "HandleCameraCommandCavehook.hpp"
#include "CommandId.hpp"
#include "InputFaker.hpp"
#include "Settings.hpp"
#include "State.hpp"
#include "Structs/Command.hpp"

void HandleCameraCommandCavehook::EnableSpecifically(intptr_t address_incl_offset)
{
    auto [prolog, epilog] = dku::Hook::JIT::MakeNonVolatilePatch({ dku::Hook::Register::ALL });
    auto handle = DKUtil::Hook::AddCaveHook(address_incl_offset, { 0, 7 }, FUNC_INFO(Func), &prolog,
        &epilog, DKUtil::Hook::HookFlag::kRestoreAfterEpilog);
    handle->Enable();
    handles.emplace_back(std::move(handle));
}

void HandleCameraCommandCavehook::Func(uint64_t* handler, WORD* result, Command* command)
{
    InputFaker::camera_command_handler_candidate = reinterpret_cast<CommandHandler*>(handler);
}
