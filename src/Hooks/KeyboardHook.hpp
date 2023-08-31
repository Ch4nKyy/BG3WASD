#pragma once

#include "../Settings.hpp"
#include "../State.hpp"
#include "../VirtualKeyMap.hpp"

#include <string>

class KeyboardHook
{
public:
    static bool Enable(HMODULE a_hModule);

private:
    static inline const DWORD CURRENT_PROCESS_ID = GetCurrentProcessId();
    static inline DWORD last_input_vk;
    static inline int last_transition;

    static LRESULT CALLBACK KeyboardProc(int a_nCode, WPARAM a_wParam, LPARAM a_lParam);
    static bool DidCommandChange(Command command, int transition);

    static void AutoRun(State* state);
    static void ToggleCharacterOrCamera(State* state);
    static void WalkOrSprint(State* state);
    static void ReloadConfig();
};
