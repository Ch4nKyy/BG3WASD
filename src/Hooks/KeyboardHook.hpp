#pragma once

#include "../Settings.hpp"
#include "../State.hpp"

#include <string>

class KeyboardHook
{
public:
    static bool Enable(HMODULE a_hModule);

private:
    static inline const DWORD CURRENT_PROCESS_ID = GetCurrentProcessId();
    static inline DWORD last_input_vk;

    static LRESULT CALLBACK KeyboardProc(int a_nCode, WPARAM a_wParam, LPARAM a_lParam);
    static bool IsCommandPressed(Command command);
    static bool IsCommandHeld(Command command);

    static void AutoRun(State* state);
    static void ToggleCharacterOrCamera(State* state);
    static void WalkOrSprint(State* state);
    static void ReloadConfig();
};
