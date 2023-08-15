#include "Hooks/KeyboardHook.hpp"
#include "Hooks/WASDUnlock.hpp"
#include "Hooks/WalkSpeedHook.hpp"
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
        Settings::GetSingleton()->Load();
        State::GetSingleton()->Load();

        INFO("Game Process Name : {}", dku::Hook::GetProcessName())

        dku::Hook::Trampoline::AllocTrampoline(1 << 7);

        WASDUnlock::Enable();
        KeyboardHook::Enable(a_hModule);
        WalkSpeedHook::Enable();
    }

    return TRUE;
}
