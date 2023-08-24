#pragma once

class State : public DKUtil::model::Singleton<State>
{
public:
    bool autorunning = false;
    bool walking = false;
    bool walking_or_sprint_held = false;
    bool is_wasd_character_movement = true;
    int frames_to_hold_forward_to_center_camera = 0;
    bool is_wasd_unlocked = true;
    bool are_extra_features_degraded = false;
    bool last_dying_character_is_player = false;
    std::string combat_end_character_name;
    bool mod_found_all_addresses = true;
    void Load();
};
