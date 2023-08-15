#pragma once

class State : public DKUtil::model::Singleton<State>
{
public:
    bool walking = false;
    bool walking_or_sprint_held = false;
    void Load();
};
