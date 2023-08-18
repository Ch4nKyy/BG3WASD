#include "State.hpp"
#include "Settings.hpp"

void State::Load()
{
    auto* settings = Settings::GetSingleton();
    walking = settings->walking_is_default;
}
