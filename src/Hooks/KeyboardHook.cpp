#include "KeyboardHook.hpp"
#include "../VirtualKeyMap.hpp"

using enum Command;

bool KeyboardHook::Enable(HMODULE a_hModule)
{
    HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, a_hModule, 0);
    if (!hHook)
    {
        WARN("KeyboardHook failed!");
        return false;
    }
    INFO("KeyboardHook succeeded.");
    return true;
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
                // Turn input vk to lowercase, so it does not depend on the Caps status!
                last_input_vk = pkbhs->vkCode;
                if (last_input_vk >= 65 && last_input_vk <= 90)
                {
                    last_input_vk += 32;
                }

                auto* state = State::GetSingleton();
                AutoRun(state);
                ToggleCharacterOrCamera(state);
                WalkOrSprint(state);
                ReloadConfig();
            }
        }
    }
    return CallNextHookEx(nullptr, a_nCode, a_wParam, a_lParam);
}

bool KeyboardHook::IsCommandPressed(Command command)
{
    bool pressed = true;
    auto* settings = Settings::GetSingleton();
    auto vkcombo = settings->GetVkComboOfCommand(command);
    for (auto vk : vkcombo)
    {
        if (vk == vkcombo.back())
        {
            pressed &= (last_input_vk == vk);
        }
        else
        {
            pressed &= ((GetKeyState(vk) & 0x8000) || (last_input_vk == vk));
        }
    }
    return pressed;
}

bool KeyboardHook::IsCommandHeld(Command command)
{
    bool held = true;
    auto* settings = Settings::GetSingleton();
    auto vkcombo = settings->GetVkComboOfCommand(command);
    for (auto vk : vkcombo)
    {
        held &= ((GetKeyState(vk) & 0x8000) || (last_input_vk == vk));
    }
    return held;
}

void KeyboardHook::AutoRun(State* state)
{
    if (IsCommandPressed(TOGGLE_AUTORUN) && state->is_wasd_character_movement)
    {
        state->autorunning = !state->autorunning;
        return;
    }

    if (IsCommandPressed(FORWARD) || IsCommandPressed(BACKWARD) ||
        IsCommandPressed(TOGGLE_CHARACTER_OR_CAMERA))
    {
        state->autorunning = false;
        return;
    }
}

void KeyboardHook::ToggleCharacterOrCamera(State* state)
{
    if (IsCommandPressed(TOGGLE_CHARACTER_OR_CAMERA))
    {
        state->is_wasd_character_movement = !state->is_wasd_character_movement;
        return;
    }
}

void KeyboardHook::WalkOrSprint(State* state)
{
    if (IsCommandPressed(TOGGLE_WALK_OR_SPRINT) && state->is_wasd_character_movement)
    {
        state->walking = !state->walking;
        return;
    }

    state->walking_or_sprint_held = IsCommandHeld(HOLD_WALK_OR_SPRINT);
}

void KeyboardHook::ReloadConfig()
{
    if (IsCommandPressed(RELOAD_CONFIG))
    {
        Settings::GetSingleton()->Load();
    }
}
