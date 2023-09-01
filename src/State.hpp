#pragma once

#include "SDL.h"

class State : public DKUtil::model::Singleton<State>
{
public:
    bool autoforward_toggled = false;
    bool walking_toggled= false;
    bool walking_held = false;
    int frames_to_hold_forward_to_center_camera = 0;
    bool last_dying_character_is_player = false;
    std::string combat_end_character_name;
    bool mod_found_all_addresses = true;
    std::vector<std::string> character_forward_keys;
    std::vector<std::string> character_backward_keys;
    std::vector<std::string> rotate_keys;
    SDL_Window* sdl_window = 0;
    bool is_rotating_changed = false;
    POINT cursor_position_to_restore;
    int frames_to_restore_cursor_pos = -1;
    bool is_mouseleft_pressed = false;
    bool old_combat_state = false;
    bool combat_state_initiliazed = false;
    uint32_t last_time_context_menu_pressed = 0;
    uint32_t last_time_cancel_action_pressed = 0;
    uint32_t rotate_start_time = 0;

    void SetIsRotating(bool in_value, bool send_fake_key = true);
    bool IsRotating();
    void SetIsWasdCharacterMovement(bool in_value);
    bool IsWasdCharacterMovement();

private:
    bool is_rotating = false;
    bool is_wasd_character_movement = true;
};
