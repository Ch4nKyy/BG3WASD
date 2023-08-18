#pragma once

#include "DKUtil/Config.hpp"

using namespace DKUtil::Alias;

enum Command
{
    TOGGLE_WALK_OR_SPRINT,
    HOLD_WALK_OR_SPRINT,
    TOGGLE_AUTORUN,
    TOGGLE_CHARACTER_OR_CAMERA,
    RELOAD_CONFIG,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Settings : public DKUtil::model::Singleton<Settings>
{
public:
    String toggle_walk_or_sprint{ "ToggleWalkOrSprint", "ModHotkeys" };
    String toggle_character_or_camera{ "ToggleCharacterOrCamera", "ModHotkeys" };
    String toggle_autorun{ "ToggleAutorun", "ModHotkeys" };
    String hold_walk_or_sprint{ "HoldWalkOrSprint", "ModHotkeys" };
    String reload_config{ "ReloadConfig", "ModHotkeys" };

    String forward{ "Forward", "GameHotkeys" };
    String backward{ "Backward", "GameHotkeys" };
    String left{ "Left", "GameHotkeys" };
    String right{ "Right", "GameHotkeys" };

    String forward_fallback{ "CameraForward", "FallbackHotkeys" };
    String backward_fallback{ "CameraBackward", "FallbackHotkeys" };
    String left_fallback{ "CameraLeft", "FallbackHotkeys" };
    String right_fallback{ "CameraRight", "FallbackHotkeys" };

    Double walk_speed{ "WalkSpeed", "Movement" };
    Boolean walking_is_default{ "WalkingIsDefault", "Movement" };

    void Load() noexcept;
    std::vector<std::uint32_t> GetVkComboOfCommand(Command command);

private:
    void AddKeyComboForCommand(Command command, String& setting);
    void LoadVkComboOfCommandMap();
    TomlConfig config = COMPILE_PROXY("NativeMods/BG3WASD.toml"sv);
    bool loaded_once = false;
    std::map<Command, std::vector<std::uint32_t>> vkcombo_of_command;
};
