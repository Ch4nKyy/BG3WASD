#pragma once

#include "DKUtil/Config.hpp"

using namespace DKUtil::Alias;

class Settings : public DKUtil::model::Singleton<Settings>
{
public:
    String toggle_walk_or_sprint{ "ToggleWalkOrSprint", "Keybinds" };
    String hold_walk_or_sprint{ "HoldWalkOrSprint", "Keybinds" };
    Double walk_speed{ "WalkSpeed", "Movement" };
    Boolean walking_is_default{ "WalkingIsDefault", "Movement" };

    void Load() noexcept;

private:
    TomlConfig config = COMPILE_PROXY("NativeMods/BG3WASD.toml"sv);
};
