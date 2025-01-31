#include "CheckCommandInputsHook.hpp"
#include "InputFaker.hpp"
#include "Settings.hpp"
#include "State.hpp"
#include "GameCommand.hpp"

using enum GameCommand;

void CheckCommandInputsHook::EnableSpecifically(uintptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

// Called in GameThread, every frame
char CheckCommandInputsHook::OverrideFunc(int64_t a1, float* a2)
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    if (state->order_force_stop)
    {
        state->is_force_stop = true;
        state->order_force_stop = false;
        InputFaker::SendCommand(ActionCancel, SDL_RELEASED);
    }

    // if (*Settings::GetSingleton()->enable_improved_mouselook)
    // {
    //     if (state->player_can_input_movement)
    //     {
    //         switch (state->mouselook_request)
    //         {
    //         case ToggleRequest::OFF:
    //             state->SetIsRotating(false);
    //             state->mouselook_toggled = false;
    //             state->mouselook_request = ToggleRequest::NONE;
    //             break;
    //         case ToggleRequest::ON:
    //             state->SetIsRotating(true);
    //             state->mouselook_toggled = true;
    //             state->mouselook_request = ToggleRequest::NONE;
    //             break;
    //         case ToggleRequest::ON_AND_OFF:  // needed when tabbing in
    //             state->SetIsRotating(true);
    //             state->SetIsRotating(false);
    //             state->mouselook_toggled = false;
    //             state->mouselook_request = ToggleRequest::NONE;
    //             break;
    //         case ToggleRequest::NONE:
    //         default:
    //             break;
    //         }
    //     }

    //     // TODO ToggleMouselook
    //     // if (state->player_could_input_movement_last_frame != state->player_can_input_movement)
    //     // {
    //     //     if (state->player_can_input_movement && state->mouselook_toggled)
    //     //     {
    //     //         state->SetIsRotating(true);
    //     //     }
    //     //     if (!state->player_can_input_movement && state->IsRotating())
    //     //     {
    //     // // TODO THIS IS PROBLEMATIC. It doesn't only stop rotate when you enter menus, but also
    //     // // when you climb steps! Funny thing is, it is somehow overwritten, when you press the
    //     // // rotate key that is bound in the second key slot in the menu!
    //     //         state->SetIsRotating(false);
    //     //     }
    //     // }
    //     state->player_could_input_movement_last_frame = state->player_can_input_movement;
    //     state->player_can_input_movement = false;
    // }

    return OriginalFunc(a1, a2);
}
