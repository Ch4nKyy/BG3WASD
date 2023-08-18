#include "Addresses/IsInControllerMode.hpp"
#include "Addresses/LoadInputConfig.hpp"
#include "Hooks/CharacterMoveInputVectorHook.hpp"
#include "Hooks/CombatEndHook.hpp"
#include "Hooks/CombatStartHook.hpp"
#include "Hooks/FTBEndHook.hpp"
#include "Hooks/FTBStartHook.hpp"
#include "Hooks/GetCameraObjectHook.hpp"
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

        dku::Hook::Trampoline::AllocTrampoline(1 << 7);

        Settings::GetSingleton()->Load();
        State::GetSingleton()->Load();

        if (WASDUnlock::Enable())
        {
            if (GetCameraObjectHook::Prepare() && CharacterMoveInputVectorHook::Prepare() &&
                KeyboardHook::Enable(a_hModule))
            {
                GetCameraObjectHook::Enable();
                CharacterMoveInputVectorHook::Enable();
            }
            else
            {
                State::GetSingleton()->are_extra_features_degraded = true;
                WARN(
                    "Extra features like walking, autorun and toggling between character and "
                    "camera movement are disabled!");
            }

            IsInControllerMode::Prepare();
            LoadInputConfig::Prepare();

            if (FTBStartHook::Prepare() && FTBEndHook::Prepare())
            {
                FTBStartHook::Enable();
                FTBEndHook::Enable();
            }
            else
            {
                WARN("Auto toggling WASD for FTB start/end disabled.");
            }
            if (CombatStartHook::Prepare() && CombatEndHook::Prepare())
            {
                CombatStartHook::Enable();
                CombatEndHook::Enable();
            }
            else
            {
                WARN("Auto toggling WASD for combat start/end disabled.");
            }
        }
        else
        {
            State::GetSingleton()->is_wasd_unlocked = false;
        }
        InputconfigPatcher::Patch();
    }

    return TRUE;
}
