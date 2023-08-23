#pragma once

#include "DKUtil/Config.hpp"

using namespace DKUtil::Alias;

class Settings : public DKUtil::model::Singleton<Settings>
{
public:
    String toggle_walk_or_sprint{ "ToggleWalkOrSprint", "ModHotkeys" };
    String toggle_character_or_camera{ "ToggleCharacterOrCamera", "ModHotkeys" };
    String toggle_autorun{ "ToggleAutorun", "ModHotkeys" };
    String hold_walk_or_sprint{ "HoldWalkOrSprint", "ModHotkeys" };
    String reload_config{ "ReloadConfig", "ModHotkeys" };
    String rotate{ "Rotate", "ModHotkeys" }; // TODO delete here and hook game fct? otherwise it must patch inputconfig, which will override it for many...

    Double walk_speed{ "WalkSpeed", "Movement" };
    Boolean walking_is_default{ "WalkingIsDefault", "Movement" };

    Boolean enable_auto_toggling_wasd_mode{ "AutoTogglingWasdMode", "EnableFeatures" };

    void Load() noexcept;

private:
    TomlConfig config = COMPILE_PROXY("NativeMods/BG3WASD.toml"sv);
    bool loaded_once = false;
};
