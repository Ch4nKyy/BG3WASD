#pragma once

class KeyboardHook
{
public:
    static void Enable(HMODULE a_hModule);

private:
    static LRESULT CALLBACK KeyboardProc(int a_nCode, WPARAM a_wParam, LPARAM a_lParam);
    static inline const DWORD CURRENT_PROCESS_ID = GetCurrentProcessId();
};
