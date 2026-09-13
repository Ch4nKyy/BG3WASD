#include "HandleCharacterCommandCavehook.hpp"
#include "CommandId.hpp"
#include "InputFaker.hpp"
#include "Settings.hpp"
#include "State.hpp"
#include "Structs/Command.hpp"

void HandleCharacterCommandCavehook::EnableSpecifically(intptr_t address_incl_offset)
{
    auto [prolog, epilog] = dku::Hook::JIT::MakeNonVolatilePatch({ dku::Hook::Register::ALL });
    auto handle = DKUtil::Hook::AddCaveHook(address_incl_offset, { 0, 6 }, FUNC_INFO(Func), &prolog,
        &epilog, DKUtil::Hook::HookFlag::kRestoreAfterEpilog);
    handle->Enable();
    handles.emplace_back(std::move(handle));
}

void HandleCharacterCommandCavehook::Func(uint64_t* handler, WORD* result, Command* command)
{
    InputFaker::character_command_handler = reinterpret_cast<CommandHandler*>(handler);

    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    if (state->IsCurrentlyInteractMoving() && state->IsCharacterMovementMode() &&
        command->id >= CharacterMoveForward && command->id <= CharacterMoveRight &&
        command->new_pressed)
    {
        state->OrderForceStop();
    }
}
