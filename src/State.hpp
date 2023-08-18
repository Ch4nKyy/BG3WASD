#pragma once

class State : public DKUtil::model::Singleton<State>
{
public:
    bool autorunning = false;
    bool walking = false;
    bool walking_or_sprint_held = false;
    bool is_wasd_character_movement = true;
    bool is_wasd_unlocked = true;
    bool are_extra_features_degraded = false;
    void Load();
};
