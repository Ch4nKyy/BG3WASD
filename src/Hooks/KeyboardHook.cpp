#include "KeyboardHook.hpp"
#include "../Settings.hpp"
#include "../State.hpp"
#include "../VirtualKeyMap.hpp"

void KeyboardHook::Enable(HMODULE a_hModule)
{
    HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, a_hModule, 0);
    if (hHook)
    {
        INFO("Keyboard Hook enabled");
    }
    else
    {
        WARN("Keyboard Hook failed! Some functionalities are disabled.");
    }
}

// https://learn.microsoft.com/de-de/windows/win32/inputdev/virtual-key-codes
LRESULT CALLBACK KeyboardHook::KeyboardProc(int a_nCode, WPARAM a_wParam, LPARAM a_lParam)
{
    if (HWND hwnd = GetForegroundWindow())
    {
        DWORD pid;
        GetWindowThreadProcessId(hwnd, &pid);
        if (pid == CURRENT_PROCESS_ID)
        {
            if (a_nCode >= 0 && a_wParam == WM_KEYDOWN)
            {
                KBDLLHOOKSTRUCT* pkbhs = (KBDLLHOOKSTRUCT*)a_lParam;
                auto* settings = Settings::GetSingleton();
                auto* state = State::GetSingleton();

                std::string toggle_walk_or_sprint_keyname = *settings->toggle_walk_or_sprint;
                int toggle_walk_or_sprint_vk =
                    VirtualKeyMap::GetByName(toggle_walk_or_sprint_keyname);
                if (pkbhs->vkCode == toggle_walk_or_sprint_vk)
                {
                    state->walking = !state->walking;
                }

                std::string hold_walk_or_sprint_keyname = *settings->hold_walk_or_sprint;
                int hold_walk_or_sprint_vk = VirtualKeyMap::GetByName(hold_walk_or_sprint_keyname);
                state->walking_or_sprint_held = GetKeyState(hold_walk_or_sprint_vk) & 0x8000;
            }
        }
    }
    return CallNextHookEx(nullptr, a_nCode, a_wParam, a_lParam);
}
