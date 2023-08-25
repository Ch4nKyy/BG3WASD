#include "Addresses/IsInControllerMode.hpp"
#include "Addresses/LoadInputConfig.hpp"
#include "Hooks/AfterChangingKeybindInMenuHook.hpp"
#include "Hooks/AfterInitialLoadInputConfigHook.hpp"
#include "Hooks/CharacterDeathHook.hpp"
#include "Hooks/CharacterMoveInputVectorHook.hpp"
#include "Hooks/CombatEndHook.hpp"
#include "Hooks/CombatStartHook.hpp"
#include "Hooks/FTBEndHook.hpp"
#include "Hooks/FTBStartHook.hpp"
#include "Hooks/GetCameraObjectHook.hpp"
#include "Hooks/GetCharacterName.hpp"
#include "Hooks/KeyboardHook.hpp"
#include "Hooks/WASDUnlock.hpp"
#include "InputconfigPatcher.hpp"
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

        dku::Hook::Trampoline::AllocTrampoline(1 << 8);

        auto settings = Settings::GetSingleton();

        settings->Load();

        std::string errors;

        bool wasd_unlock = WASDUnlock::Prepare();
        bool keyboard_hook = KeyboardHook::PrepareAndEnable(a_hModule);
        bool get_camera_object_hook = GetCameraObjectHook::Prepare();
        bool character_movement_input_vector_hook = CharacterMoveInputVectorHook::Prepare();
        bool is_in_controller_mode = IsInControllerMode::Prepare();
        bool load_input_config = LoadInputConfig::Prepare();
        bool after_changing_keybind_in_menu_hook = AfterChangingKeybindInMenuHook::Prepare();
        bool after_initial_load_inputconfig_hook = AfterInitialLoadInputConfigHook::Prepare();
        if (wasd_unlock && keyboard_hook && get_camera_object_hook &&
            character_movement_input_vector_hook && is_in_controller_mode &&
            after_changing_keybind_in_menu_hook && load_input_config &&
            after_initial_load_inputconfig_hook)
        {
            WASDUnlock::Enable();
            GetCameraObjectHook::Enable();
            CharacterMoveInputVectorHook::Enable();
            AfterChangingKeybindInMenuHook::Enable();
            AfterInitialLoadInputConfigHook::Enable();

            bool ftb_start_hook = FTBStartHook::Prepare();
            bool ftb_end_hook = FTBEndHook::Prepare();
            if (settings->enable_auto_toggling_wasd_mode && ftb_start_hook && ftb_end_hook)
            {
                FTBStartHook::Enable();
                FTBEndHook::Enable();
            }
            else
            {
                if (settings->enable_auto_toggling_wasd_mode)
                {
                    errors.append("Auto toggling WASD at FTB start/end disabled.\n");
                }
            }

            bool character_death_hook = CharacterDeathHook::Prepare();
            bool combat_start_hook = CombatStartHook::Prepare();
            bool combat_end_hook = CombatEndHook::Prepare();
            bool get_character_name = GetCharacterName::Prepare();
            if (settings->enable_auto_toggling_wasd_mode && character_death_hook &&
                combat_start_hook && combat_end_hook && get_character_name)
            {
                CombatStartHook::Enable();
                CombatEndHook::Enable();
                CharacterDeathHook::Enable();
                GetCharacterName::Enable();
            }
            else
            {
                if (settings->enable_auto_toggling_wasd_mode)
                {
                    errors.append("Auto toggling WASD at combat start/end disabled.\n");
                }
            }
        }
        else
        {
            errors.append("WASD could not be unlocked at all! Mod will be inactive.\n");
        }

        if (!State::GetSingleton()->mod_found_all_addresses)
        {
            errors.append("Your game version is not completely compatible with the mod version!\n");
        }

        if (!errors.empty())
        {
            WARN(errors);
            // TODO also display a box to the player at main menu.
        }
    }

    return TRUE;
}
