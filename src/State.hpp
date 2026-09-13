#pragma once

#include "RotationCursorType.hpp"
#include "SDL.h"
#include "VirtualCursorState.hpp"

struct Camera;

class State : public DKUtil::model::Singleton<State>
{
public:
    bool mod_found_all_addresses = true;

    // core

    bool autoforward_toggled = false;

    bool walking_toggled = false;

    bool walking_held = false;

    // If you set this to ~10, the mod will input character forward movement for 10 frames, which is enough to center
    // the camera on the character.
    int frames_to_hold_forward_to_center_camera = 0;

    std::vector<std::string> character_forward_keys;

    std::vector<std::string> character_backward_keys;

    // Capture the inputconfig_path, so that we can use it to find the inputconfig file in the future.
    std::wstring inputconfig_path = L"";

    // Keeps track of whether the player is seeing the world context menu.
    bool is_context_menu_open = false;

    // Keeps track of whether the player is seeing at least 1 menu, e.g. inventory, map, examine window, etc.
    bool is_menu_open = false;

    // Keeps track of whether the player is in a dialog or cutscene.
    bool is_in_dialog = false;

    // Keeps track of whether at least 1 loot window is open.
    bool is_loot_window_open = false;

    // Usually, the camera rotation command is blocked during camera rotation toggle, but for the very first frame,
    // it must be allowed, otherwise the camera rotation will not start at all. This variable is used to track that
    // first frame.
    bool camera_rotation_toggled_this_frame = false;

    // The TogglingMovementModeAlsoTogglesCameraRotation feature has an issue where it is not correctly engaged
    // at the first load of the game.
    // For some reason, when sending the camera rotation command, the game does not reach the CameraRotationOnHook.
    // In that case we have to force a camera rotation update on the first camera input of the game.
    // On subsequent loads, it is no issue.
    bool is_first_camera_input_of_the_game = true;

    void SetMovementModeToggled(bool in_value);
    void SetMovementModeHeld(bool in_value);

    // Returns the current camera.
    // CAN RETURN NULLPTR!
    // In rare cases, when you press rotate camera, the camera can be buggy and contain invalid data.
    // This function tries to detect that and return a nullptr in that case.
    // It's also thinkable that we rarely update the camera pointer with a wrong camera, so
    // we also perform these checks in SetCamera, but I didn't observe that happening yet.
    Camera* GetCamera();
    void SetCamera(Camera* in_camera);
    bool IsCameraValid(Camera* camera);

    // Keeps track of whether camera occluders were prevented in the last frame.
    // Why is that useful? In very rare cases the states camera contains invalid data and thus becomes invalid itself.
    // In that case, camera occluders might be clickable for one frame, although they shouldn't be.
    // When the camera is invalid, instead of allowing clicks, remember the last frame.
    bool prevented_camera_occluders_last_frame = false;
    float last_valid_camera_zoom = 0.0f;

    void UpdateMovementMode();
    bool IsCharacterMovementMode();
    bool IsMovementModeToggled();
    void CenterCamera();
    bool IsInMainMenu();

    int64_t active_character = 0x0FFC000000000000;

    // auto toggle movement

    bool is_in_combat = false;
    bool combat_state_initialized = false;
    uint32_t last_time_combat_state_changed = 0;

    // camera rotation

    // Flags the current camera rotation toggle state as dirty, so that camera rotation will be updated next frame.
    // Only updates if necessary, to avoid unnecessary camera rotation toggling, but might sometimes not be enough
    // to end up in the correct state, so we have is_camera_rotation_toggle_very_dirty as well.
    bool is_camera_rotation_toggle_dirty = false;

    // Does the same as is_camera_rotation_toggle_dirty, but always updates the state.
    bool is_camera_rotation_toggle_very_dirty = false;

    bool rotate_keys_include_lmb = false;
    SDL_Window* sdl_window = 0;
    POINT cursor_position_to_restore;
    bool is_mouseleft_pressed = false;
    bool last_frame_rotate_and_lmb_was_pressed = false;
    bool cursor_overriden_last_frame = false;
    std::mutex override_cursor_mutex;
    RotationCursorType rotation_cursor_in_character_mode = RotationCursorType::INVISIBLE;
    RotationCursorType rotation_cursor_in_camera_mode = RotationCursorType::INVISIBLE;
    bool is_in_photomode = false;
    VirtualCursorState virtual_cursor_state = VirtualCursorState::NORMAL;
    POINT virtual_cursor_position;

    // When you switch movement mode while holding Camera Rotate button, there currently is an issue when the
    // rotation cursor transitions from Vanilla to Invisible/Centered. The cursor will get stuck invisible.
    // I don't want to spend more time investigating this, so I just created this workaround.
    bool dont_restart_camera = false;

    bool IsCameraRotationToggled();
    void EnableCameraRotation(bool in_value);
    void SetInternalCameraRotationState(bool in_value);
    bool IsRotating();
    void OverrideCursor(bool in_value);
    bool ShouldOverrideCursor();
    void OverrideRotationCursorIcon(bool enabled);
    bool IsPlayerInputBlocked();
    void ToggleCameraRotation();
    void UpdateBlockCancelActionStoppingMovementPatch();

    // Calling this function is necessary when you switch rotation cursors while you are rotating.
    // Otherwise, the cursor will not update correctly.
    void RestartCameraRotation();

    // interactmove disabler

    void OrderForceStop();
    bool is_force_stop = false;

    void EnableInteractMoveBlocker(bool enabled);
    void SetCurrentlyInteractMoving(bool in_value);
    bool IsCurrentlyInteractMoving();

    RotationCursorType GetRotationCursorTypeInCurrentMode();

private:
    //core

    bool movement_mode_toggled = true;
    bool movement_mode_held = false;

    // camera rotation

    bool is_camera_rotating = false;
    bool should_override_cursor = false;
    bool is_camera_rotation_toggled = false;
    Camera* camera;

    // InteractMoveBlocker

    bool currently_interact_moving = false;
};
