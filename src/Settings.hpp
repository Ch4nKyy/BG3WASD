#pragma once

#include "DKUtil/Config.hpp"

using namespace DKUtil::Alias;

class Settings : public DKUtil::model::Singleton<Settings>
{
public:
    String toggle_walkspeed{ "ToggleWalkspeed", "ModHotkeys" };
    String toggle_movement_mode{ "ToggleMovementMode", "ModHotkeys" };
    String hold_movement_mode{ "HoldMovementMode", "ModHotkeys" };
    String toggle_autoforward{ "ToggleAutoforward", "ModHotkeys" };
    String hold_walkspeed{ "HoldWalkspeed", "ModHotkeys" };
    String reload_config{ "ReloadConfig", "ModHotkeys" };
    String toggle_camerarotation{ "ToggleCameraRotation", "ModHotkeys" };

    Double walk_speed{ "WalkSpeed", "Walking" };
    Boolean is_walking_default{ "IsWalkingDefault", "Walking" };
    Boolean walk_after_combat{ "SwitchToWalkingAfterCombat", "Walking" };

    Boolean auto_toggle_movement_mode{ "AutoToggleMovementMode", "AutoToggleMovementMode" };
    Boolean is_character_movement_mode_default{ "IsCharacterMovementDefault",
        "AutoToggleMovementMode" };

    // I don't include this setting in the shipped toml file, because I think there isn't really a reason to turn it
    // off, and then the settings file is leaner.
    Boolean rotate_plus_lmb_is_forward{ "RotatePlusLeftclickMovesForward", "CameraRotation" };

    Boolean toggling_movement_mode_also_toggles_camera_rotation{
        "TogglingMovementModeAlsoTogglesCameraRotation", "CameraRotation"
    };

    Boolean block_interact_move{ "BlockInteractMove", "Miscellaneous" };

    Boolean prevent_clicking_active_character{ "PreventClickingActiveCharacter", "Miscellaneous" };

    Boolean prevent_clicking_camera_occluders{
        "PreventClickingCameraOccluders", "Miscellaneous"
    };

    void Load() noexcept;
    std::vector<std::string> GetBoundKeycombos(std::string setting);

private:
    TomlConfig config = COMPILE_PROXY("NativeMods/BG3WASD.toml"sv);
    bool first_time_loaded = true;

    // For performance reasons, we convert this into an enum and store it in the State.
    String rotation_cursor_in_character_mode{ "CharacterMovementRotationCursor", "CameraRotation" };
    // For performance reasons, we convert this into an enum and store it in the State.
    String rotation_cursor_in_camera_mode{ "CameraMovementRotationCursor", "CameraRotation" };

    void InitState();
};
