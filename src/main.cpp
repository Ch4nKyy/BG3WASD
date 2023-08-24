#include "Addresses/IsInControllerMode.hpp"
#include "Addresses/LoadInputConfig.hpp"
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

        Settings::GetSingleton()->Load();
        State::GetSingleton()->Load();

        if (WASDUnlock::Enable())
        {
            bool get_camera_object_hook = GetCameraObjectHook::Prepare();
            bool character_movement_input_vector_hook = CharacterMoveInputVectorHook::Prepare();
            bool keyboard_hook = KeyboardHook::Enable(a_hModule);
            if (get_camera_object_hook && character_movement_input_vector_hook && keyboard_hook)
            {
                GetCameraObjectHook::Enable();
                CharacterMoveInputVectorHook::Enable();

                IsInControllerMode::Prepare();
                LoadInputConfig::Prepare();

                bool ftb_start_hook = FTBStartHook::Prepare();
                bool ftb_end_hook = FTBEndHook::Prepare();
                if (ftb_start_hook && ftb_end_hook)
                {
                    FTBStartHook::Enable();
                    FTBEndHook::Enable();
                }
                else
                {
                    WARN("Auto toggling WASD for FTB start/end disabled.");
                }

                bool character_death_hook = CharacterDeathHook::Prepare();
                bool combat_start_hook = CombatStartHook::Prepare();
                bool combat_end_hook = CombatEndHook::Prepare();
                bool get_character_name = GetCharacterName::Prepare();
                if (character_death_hook && combat_start_hook && combat_end_hook &&
                    get_character_name)
                {
                    CombatStartHook::Enable();
                    CombatEndHook::Enable();
                    CharacterDeathHook::Enable();
                    GetCharacterName::Enable();
                }
                else
                {
                    WARN("Auto toggling WASD for combat start/end disabled.");
                }
            }
            else
            {
                State::GetSingleton()->are_extra_features_degraded = true;
                WARN(
                    "Extra features like walking, autorun and toggling WASD between character and "
                    "camera movement are disabled!");
            }
        }
        else
        {
            State::GetSingleton()->is_wasd_unlocked = false;
        }

        InputconfigPatcher::Patch();

        if (!State::GetSingleton()->mod_found_all_addresses)
        {
            WARN("Your game version is not completely compatible with the mod version!");
        }
    }

    return TRUE;
}
