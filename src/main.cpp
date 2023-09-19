#include "Addresses/IsInControllerMode.hpp"
#include "Addresses/LoadInputConfig.hpp"
#include "Hooks/AfterChangingKeybindInMenuHook.hpp"
#include "Hooks/AfterInitialLoadInputConfigHook.hpp"
#include "Hooks/CallSpecificCommandFunctionPre2Hook.hpp"
#include "Hooks/CastOrCancelAbilityHook.hpp"
#include "Hooks/CheckCommandInputsHook.hpp"
#include "Hooks/CheckContextMenuOrCancelActionHook.hpp"
#include "Hooks/CombatEndHook.hpp"
#include "Hooks/CombatStartHook.hpp"
#include "Hooks/DecideMoveUpdaterHook.hpp"
#include "Hooks/FTBEndHook.hpp"
#include "Hooks/FTBStartHook.hpp"
#include "Hooks/GetCameraObjectHook.hpp"
#include "Hooks/GetInputValueHook.hpp"
#include "Hooks/InputHook.hpp"
#include "Hooks/InsideUpdateInteractMoveHook.hpp"
#include "Hooks/LoadStringHook.hpp"
#include "Hooks/PollEventHook.hpp"
#include "Hooks/ResetCursorRotateHook.hpp"
#include "Hooks/SDL_GetWindowGrabHook.hpp"
#include "Hooks/SetCursorRotateHook.hpp"
#include "Hooks/SetVirtualCursorPosHook.hpp"
#include "Hooks/WASDUnlock.hpp"
#include "Hooks/WindowGainFocusHook.hpp"
#include "InputconfigPatcher.hpp"
#include "Patches/BlockAnalogStickSelection2Patch.hpp"
#include "Patches/BlockAnalogStickSelectionPatch.hpp"
#include "Patches/BlockCancelActionStoppingMovementPatch.hpp"
#include "Patches/BlockHoldInteractMovePatch.hpp"
#include "Patches/BlockInteractMovePatch.hpp"
#include "SDL.h"
#include "Settings.hpp"
#include "State.hpp"
#include "VersionInfo.hpp"

BOOL APIENTRY DllMain(HMODULE a_hModule, DWORD a_ul_reason_for_call, LPVOID a_lpReserved)
{
    if (a_ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
#ifndef NDEBUG
        while (!IsDebuggerPresent())
        {
            Sleep(100);
        }
#endif
        dku::Logger::Init(Plugin::NAME, std::to_string(Plugin::Version));
        dku::Hook::Trampoline::AllocTrampoline(1 << 9);

        VersionInfo::Print(a_hModule);

        SDL_version linked;
        SDL_GetVersion(&linked);
        if (linked.major != 2 || linked.minor != 28)
        {
            FATAL(
                "SDL2.dll version mismatch. This usually means that you must re-install the mod, "
                "or at least the SDL2.dll!");
        }
        // With SDL 2.24 and spam-clicking Rotate I could bug the cursor so that it would
        // warp to the center of the screen.
        // Using SDL 2.28 and this Hint fixes this issue.
        SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_CENTER, "0");

        auto* settings = Settings::GetSingleton();
        settings->Load();

        std::string errors;

        bool wasd_unlock = WASDUnlock::Prepare();
        bool get_camera_object_hook = GetCameraObjectHook::Prepare();
        bool character_movement_input_vector_hook = GetInputValueHook::Prepare();
        bool is_in_controller_mode = IsInControllerMode::Prepare();
        bool load_input_config = LoadInputConfig::Prepare();
        bool after_changing_keybind_in_menu_hook = AfterChangingKeybindInMenuHook::Prepare();
        bool after_initial_load_inputconfig_hook = AfterInitialLoadInputConfigHook::Prepare();
        bool load_string_hook = LoadStringHook::Prepare();
        bool block_analog_stick_selection_patch = BlockAnalogStickSelectionPatch::Prepare();
        bool block_analog_stick_selection_patch2 = BlockAnalogStickSelection2Patch::Prepare();
        if (wasd_unlock && get_camera_object_hook && character_movement_input_vector_hook &&
            is_in_controller_mode && after_changing_keybind_in_menu_hook && load_input_config &&
            after_initial_load_inputconfig_hook && load_string_hook &&
            block_analog_stick_selection_patch && block_analog_stick_selection_patch2)
        {
            InputHook::Enable(a_hModule);  // throws on error
            WASDUnlock::Enable();
            GetCameraObjectHook::Enable();
            GetInputValueHook::Enable();
            AfterChangingKeybindInMenuHook::Enable();
            AfterInitialLoadInputConfigHook::Enable();
            LoadStringHook::Enable();
            BlockAnalogStickSelectionPatch::Enable();
            BlockAnalogStickSelectionPatch::Activate();
            BlockAnalogStickSelection2Patch::Enable();
            BlockAnalogStickSelection2Patch::Activate();

            bool ftb_start_hook = FTBStartHook::Prepare();
            bool ftb_end_hook = FTBEndHook::Prepare();
            if (ftb_start_hook && ftb_end_hook)
            {
                FTBStartHook::Enable();
                FTBEndHook::Enable();
            }
            else
            {
                errors.append(
                    "Auto toggling movement mode at forced turn-based mode start/end could not be "
                    "enabled.\n");
            }

            bool set_virtual_cursor_pos_hook = SetVirtualCursorPosHook::Prepare();
            bool get_window_grab_hook = SDL_GetWindowGrabHook::Prepare();
            bool set_cursor_rotate_hook = SetCursorRotateHook::Prepare();
            bool reset_cursor_rotate_hook = ResetCursorRotateHook::Prepare();
            bool check_command_inputs_hook = CheckCommandInputsHook::Prepare();
            bool check_context_menu_or_cancel_action_hook =
                CheckContextMenuOrCancelActionHook::Prepare();
            bool cast_or_cancel_ability_hook = CastOrCancelAbilityHook::Prepare();
            bool poll_event_hook = PollEventHook::Prepare();
            // TODO ToggleMouselook
            // bool windows_gain_focus_hook = WindowGainFocusHook::Prepare();
            if (set_virtual_cursor_pos_hook && get_window_grab_hook && set_cursor_rotate_hook &&
                reset_cursor_rotate_hook && check_command_inputs_hook &&
                check_context_menu_or_cancel_action_hook && cast_or_cancel_ability_hook &&
                poll_event_hook)
            {
                SetVirtualCursorPosHook::Enable();
                SDL_GetWindowGrabHook::Enable();
                SetCursorRotateHook::Enable();
                ResetCursorRotateHook::Enable();
                CheckCommandInputsHook::Enable();
                CheckContextMenuOrCancelActionHook::Enable();
                CastOrCancelAbilityHook::Enable();
                PollEventHook::Enable();
                // TODO ToggleMouselook
                // WindowGainFocusHook::Enable();
            }
            else
            {
                errors.append("Improved Mouselook could not be enabled.\n");
            }

            bool decide_move_updater_hook = DecideMoveUpdaterHook::Prepare();
            bool inside_update_interact_move_hook = InsideUpdateInteractMoveHook::Prepare();
            bool call_specific_command_function_pre2_hook =
                CallSpecificCommandFunctionPre2Hook::Prepare();
            bool block_interact_move_patch = BlockInteractMovePatch::Prepare();
            bool block_hold_interact_move_patch = BlockHoldInteractMovePatch::Prepare();
            bool block_cancel_stopping_movement_patch =
                BlockCancelActionStoppingMovementPatch::Prepare();
            if (decide_move_updater_hook && inside_update_interact_move_hook &&
                call_specific_command_function_pre2_hook && block_interact_move_patch &&
                block_hold_interact_move_patch && block_cancel_stopping_movement_patch)
            {
                DecideMoveUpdaterHook::Enable();
                InsideUpdateInteractMoveHook::Enable();
                CallSpecificCommandFunctionPre2Hook::Enable();
                BlockInteractMovePatch::Enable();
                BlockHoldInteractMovePatch::Enable();
                BlockCancelActionStoppingMovementPatch::Enable();
                // The blocker patches are initialized in GetCameraObjectHook.
            }
            else
            {
                errors.append("InteractMoveBlocker could not be enabled.\n");
            }
        }
        else
        {
            errors.append(
                "Character movement could not be enabled at all! Mod will be inactive.\n");
        }

        if (!State::GetSingleton()->mod_found_all_addresses)
        {
            errors.append("Your game version is not completely compatible with the mod version!\n");
        }

        if (!errors.empty())
        {
            WARN(errors);
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "BG3WASD", errors.c_str(), NULL);
        }
        else
        {
            INFO("WASD Mod started without errors.");
        }
    }

    return TRUE;
}
