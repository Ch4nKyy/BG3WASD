#pragma once

class State : public DKUtil::model::Singleton<State>
{
public:
    bool autorunning = false;
    bool walking = false;
    bool walking_or_sprint_held = false;
    bool is_wasd_character_movement = true;
    int frames_to_hold_forward_to_center_camera = 0;
    bool last_dying_character_is_player = false;
    std::string combat_end_character_name;
    bool mod_found_all_addresses = true;
    std::vector<std::string> character_forward_keys;
    std::vector<std::string> character_backward_keys;
    std::vector<std::string> rotate_keys;
    int64_t sdl_window_ptr = 0;
    bool is_mouselook = false;
    POINT cursor_position_to_restore;
    bool mouselook_changed = false;
    int frames_to_restore_cursor_pos = 0;
    bool is_mouseleft_pressed = false;
};
