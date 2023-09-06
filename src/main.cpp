#include "Addresses/IsInControllerMode.hpp"
#include "Addresses/LoadInputConfig.hpp"
#include "Hooks/AfterChangingKeybindInMenuHook.hpp"
#include "Hooks/AfterInitialLoadInputConfigHook.hpp"
#include "Hooks/CheckCommandInputsHook.hpp"
#include "Hooks/CheckContextMenuOrCancelActionHook.hpp"
#include "Hooks/CombatEndHook.hpp"
#include "Hooks/CombatStartHook.hpp"
#include "Hooks/FTBEndHook.hpp"
#include "Hooks/FTBStartHook.hpp"
#include "Hooks/GetCameraObjectHook.hpp"
#include "Hooks/GetInputValueHook.hpp"
#include "Hooks/InputHook.hpp"
#include "Hooks/ResetCursorRotateHook.hpp"
#include "Hooks/SDL_GetWindowGrabHook.hpp"
#include "Hooks/SetCursorRotateHook.hpp"
#include "Hooks/SetVirtualCursorPosHook.hpp"
#include "Hooks/WASDUnlock.hpp"
#include "Hooks/WindowGainFocusHook.hpp"
#include "InputconfigPatcher.hpp"
#include "SDL.h"
#include "Settings.hpp"
#include "State.hpp"

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

        INFO("Game Process Name : {}", dku::Hook::GetProcessName())

        SDL_version linked;
        SDL_GetVersion(&linked);
        if (linked.major != 2 || linked.minor != 24)
        {
            FATAL(
                "SDL2.dll version mismatch. This usually means that you must re-install the mod!");
        }

        dku::Hook::Trampoline::AllocTrampoline(1 << 9);

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
        if (wasd_unlock && get_camera_object_hook && character_movement_input_vector_hook &&
            is_in_controller_mode && after_changing_keybind_in_menu_hook && load_input_config &&
            after_initial_load_inputconfig_hook)
        {
            InputHook::Enable(a_hModule);  // throws on error
            WASDUnlock::Enable();
            GetCameraObjectHook::Enable();
            GetInputValueHook::Enable();
            AfterChangingKeybindInMenuHook::Enable();
            AfterInitialLoadInputConfigHook::Enable();

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
            // TODO ToggleMouselook
            // bool windows_gain_focus_hook = WindowGainFocusHook::Prepare();
            if (set_virtual_cursor_pos_hook && get_window_grab_hook && set_cursor_rotate_hook &&
                reset_cursor_rotate_hook && check_command_inputs_hook &&
                check_context_menu_or_cancel_action_hook)
            {
                SetVirtualCursorPosHook::Enable();
                SDL_GetWindowGrabHook::Enable();
                SetCursorRotateHook::Enable();
                ResetCursorRotateHook::Enable();
                CheckCommandInputsHook::Enable();
                CheckContextMenuOrCancelActionHook::Enable();
                // TODO ToggleMouselook
                // WindowGainFocusHook::Enable();
            }
            else
            {
                errors.append("Improved Mouselook could not be enabled.\n");
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
    }

    return TRUE;
}
