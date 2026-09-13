#include "ExecuteCameraCommandHook.hpp"
#include "CommandId.hpp"
#include "InputFaker.hpp"
#include "Settings.hpp"
#include "State.hpp"
#include "Structs/Camera.hpp"
#include "Structs/Command.hpp"

void ExecuteCameraCommandHook::EnableSpecifically(intptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

char* ExecuteCameraCommandHook::OverrideFunc(uint64_t executor, char* result, void* context,
    Command* command)
{
    auto* settings = Settings::GetSingleton();
    auto* state = State::GetSingleton();

    InputFaker::camera_command_handler = InputFaker::camera_command_handler_candidate;

    if (command->id >= CameraBackward && command->id <= CameraRight)
    {
        if (state->IsCharacterMovementMode() && !state->is_in_combat)
        {
            command->id = 0;
        }
    }

    // During Camera Rotation toggle, block the CameraToggleMouseRotate command.
    // Except for the very first time/frame, because that actually toggles the rotation.
    // It's not super important, just some polish.
    if (state->IsCameraRotationToggled() && !state->camera_rotation_toggled_this_frame &&
        command->id == CameraToggleMouseRotate)
    {
        command->id = 0;
    }

    // Prevent a bug. See doc of is_first_camera_input_of_the_game.
    if (*settings->toggling_movement_mode_also_toggles_camera_rotation &&
        state->is_first_camera_input_of_the_game)
    {
        state->is_first_camera_input_of_the_game = false;
        state->is_camera_rotation_toggle_dirty = true;
    }

    return OriginalFunc(executor, result, context, command);
}
