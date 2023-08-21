#include "KeyboardHook.hpp"
#include "../VirtualKeyMap.hpp"

using enum Command;

bool KeyboardHook::Enable(HMODULE a_hModule)
{
    HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, a_hModule, 0);
    if (!hHook)
    {
        WARN("KeyboardHook not found!");
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
            if (a_nCode >= 0 && a_wParam == WM_KEYDOWN || a_wParam == WM_KEYUP)
            {
                // lower key codes conflict with other codes, wtf, so use upper
                last_input_vk = ((KBDLLHOOKSTRUCT*)a_lParam)->vkCode;
                last_transition = a_wParam;
                
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

bool KeyboardHook::DidCommandChange(Command command, int transition)
{
    if (last_transition != transition)
    {
        return false;
    }
    bool changed = true;
    auto* settings = Settings::GetSingleton();
    auto vkcombo = settings->GetVkComboOfCommand(command);
    for (auto vk : vkcombo)
    {
        if (vk == vkcombo.back())
        {
            changed &= (last_input_vk == vk);
        }
        else
        {
            changed &= ((GetAsyncKeyState(vk) & 0x8000) || (last_input_vk == vk));
        }
    }
    return changed;
}

void KeyboardHook::AutoRun(State* state)
{
    if (DidCommandChange(TOGGLE_AUTORUN, WM_KEYDOWN) && state->is_wasd_character_movement)
    {
        state->autorunning = !state->autorunning;
        return;
    }

    if (DidCommandChange(FORWARD, WM_KEYDOWN) || DidCommandChange(BACKWARD, WM_KEYDOWN) ||
        DidCommandChange(TOGGLE_CHARACTER_OR_CAMERA, WM_KEYDOWN))
    {
        state->autorunning = false;
        return;
    }
}

void KeyboardHook::ToggleCharacterOrCamera(State* state)
{
    if (DidCommandChange(TOGGLE_CHARACTER_OR_CAMERA, WM_KEYDOWN))
    {
        state->is_wasd_character_movement = !state->is_wasd_character_movement;
        if (state->is_wasd_character_movement)
        {
            state->frames_to_hold_forward_to_center_camera = 10;
        }
        return;
    }
}

void KeyboardHook::WalkOrSprint(State* state)
{
    if (DidCommandChange(TOGGLE_WALK_OR_SPRINT, WM_KEYDOWN) && state->is_wasd_character_movement)
    {
        state->walking = !state->walking;
        return;
    }

    if (DidCommandChange(HOLD_WALK_OR_SPRINT, WM_KEYDOWN) && state->is_wasd_character_movement)
    {
        state->walking_or_sprint_held = true;
        return;
    }

    if (DidCommandChange(HOLD_WALK_OR_SPRINT, WM_KEYUP) && state->is_wasd_character_movement)
    {
        state->walking_or_sprint_held = false;
        return;
    }
}

void KeyboardHook::ReloadConfig()
{
    if (DidCommandChange(RELOAD_CONFIG, WM_KEYDOWN))
    {
        Settings::GetSingleton()->Load();
    }
}
