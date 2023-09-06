#pragma once

#include "SDL.h"
#include "ToggleRequest.hpp"

class State : public DKUtil::model::Singleton<State>
{
public:
    bool mod_found_all_addresses = true;

    // core
    bool autoforward_toggled = false;
    bool walking_toggled = false;
    bool walking_held = false;
    int frames_to_hold_forward_to_center_camera = 0;
    std::vector<std::string> character_forward_keys;
    std::vector<std::string> character_backward_keys;

    void SetIsWasdCharacterMovement(bool in_value);
    bool IsWasdCharacterMovement();

    // auto toggle movement
    bool last_dying_character_is_player = false;
    std::string combat_end_character_name;
    bool old_combat_state = false;
    bool combat_state_initiliazed = false;

    // mouselook
    bool mouselook_toggled = false;
    std::vector<std::string> rotate_keys;
    SDL_Window* sdl_window = 0;
    bool is_rotating_changed = false;
    bool set_is_rotating_was_faked = false;
    POINT cursor_position_to_restore;
    int frames_to_restore_cursor_pos = -1;
    bool is_mouseleft_pressed = false;
    uint32_t rotate_start_time = 0;
    uint32_t last_time_context_menu_pressed = 0;
    uint32_t last_time_cancel_action_pressed = 0;
    bool player_can_input_movement = false;
    bool player_could_input_movement_last_frame = false;
    ToggleRequest mouselook_request = ToggleRequest::NONE;
    bool last_frame_rotate_and_lmb_was_pressed = false;

    void SetIsRotating(bool in_value);
    void SetInternalIsRotating(bool in_value);
    bool IsRotating();

private:
    //core
    bool is_wasd_character_movement = true;

    // mouselook
    bool is_rotating = false;
};
