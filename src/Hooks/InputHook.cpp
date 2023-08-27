#include "InputHook.hpp"
#include "../VirtualKeyMap.hpp"

using enum Command;

void InputHook::Enable(HMODULE a_hModule)
{
    std::thread([](HMODULE a_hModule) { StartHookAsOwnThread(a_hModule); }, a_hModule).detach();
}

void InputHook::StartHookAsOwnThread(HMODULE a_hModule)
{
    HHOOK keyboard_hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, a_hModule, 0);
    HHOOK mouse_hook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, a_hModule, 0);
    if (!keyboard_hook)
    {
        FATAL("Critical error. Keyboard hook failed!");
    }
    if (!mouse_hook)
    {
        FATAL("Critical error. Mouse hook failed!");
    }
    MSG msg;

    while (GetMessage(&msg, 0, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(keyboard_hook);
    UnhookWindowsHookEx(mouse_hook);
}

LRESULT CALLBACK InputHook::MouseProc(int a_nCode, WPARAM a_wParam, LPARAM a_lParam)
{
    if (HWND hwnd = GetForegroundWindow())
    {
        DWORD pid;
        GetWindowThreadProcessId(hwnd, &pid);
        if (pid == CURRENT_PROCESS_ID)
        {
            DWORD mouseData = ((MSLLHOOKSTRUCT*)a_lParam)->mouseData;
            if (a_nCode >= 0 && SetLastInputVkByMouseInput(a_wParam, mouseData))
            {
                HandleInput();
            }
        }
    }
    return CallNextHookEx(nullptr, a_nCode, a_wParam, a_lParam);
}

// https://learn.microsoft.com/de-de/windows/win32/inputdev/virtual-key-codes
LRESULT CALLBACK InputHook::KeyboardProc(int a_nCode, WPARAM a_wParam, LPARAM a_lParam)
{
    if (HWND hwnd = GetForegroundWindow())
    {
        DWORD pid;
        GetWindowThreadProcessId(hwnd, &pid);
        if (pid == CURRENT_PROCESS_ID)
        {
            if (a_nCode >= 0 && (a_wParam == WM_KEYDOWN || a_wParam == WM_KEYUP))
            {
                // lower key codes conflict with other codes, wtf, so use upper
                last_input_vk = ((KBDLLHOOKSTRUCT*)a_lParam)->vkCode;
                last_transition = a_wParam;
                HandleInput();
            }
        }
    }
    return CallNextHookEx(nullptr, a_nCode, a_wParam, a_lParam);
}

void InputHook::HandleInput()
{
    auto* state = State::GetSingleton();
    AutoRun(state);
    ToggleCharacterOrCamera(state);
    WalkOrSprint(state);
    ReloadConfig();
    Rotate();
    MouseLeftDown();
}

bool InputHook::DidCommandChange(Command command, int transition)
{
    if (last_transition != transition)
    {
        return false;
    }
    auto vkcombos = VirtualKeyMap::GetVkCombosOfCommand(command);
    for (auto vkcombo : vkcombos)
    {
        bool changed = true;
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
        if (changed)
        {
            return true;
        }
    }
    return false;
}

void InputHook::AutoRun(State* state)
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

void InputHook::ToggleCharacterOrCamera(State* state)
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

void InputHook::WalkOrSprint(State* state)
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

void InputHook::ReloadConfig()
{
    if (DidCommandChange(RELOAD_CONFIG, WM_KEYDOWN))
    {
        Settings::GetSingleton()->Load();
    }
}

void InputHook::Rotate()
{
    auto state = State::GetSingleton();
    if (DidCommandChange(ROTATE, WM_KEYDOWN) && !state->is_rotating &&
        Settings::GetSingleton()->enable_improved_mouse_rotation)
    {
        state->is_rotating = true;
        state->is_rotating_changed = true;
        GetCursorPos(&state->cursor_position_to_restore);
    }
    if (DidCommandChange(ROTATE, WM_KEYUP) && state->is_rotating &&
        Settings::GetSingleton()->enable_improved_mouse_rotation)
    {
        state->is_rotating = false;
        state->is_rotating_changed = true;
        state->frames_to_restore_cursor_pos = 2;
    }
}

void InputHook::MouseLeftDown()
{
    auto state = State::GetSingleton();
    if (DidCommandChange(MOUSE_LEFT_DOWN, WM_KEYDOWN))
    {
        state->is_mouseleft_pressed = true;
    }
    if (DidCommandChange(MOUSE_LEFT_DOWN, WM_KEYUP))
    {
        state->is_mouseleft_pressed = false;
    }
}

bool InputHook::SetLastInputVkByMouseInput(WPARAM wParam, DWORD mouseData)
{
    bool event_was_button_press_or_release = true;
    switch (wParam)
    {
    case WM_LBUTTONDOWN:
        last_input_vk = VK_LBUTTON;
        last_transition = WM_KEYDOWN;
        break;
    case WM_LBUTTONUP:
        last_input_vk = VK_LBUTTON;
        last_transition = WM_KEYUP;
        break;
    case WM_MBUTTONDOWN:
        last_input_vk = VK_MBUTTON;
        last_transition = WM_KEYDOWN;
        break;
    case WM_MBUTTONUP:
        last_input_vk = VK_MBUTTON;
        last_transition = WM_KEYUP;
        break;
    case WM_RBUTTONDOWN:
        last_input_vk = VK_RBUTTON;
        last_transition = WM_KEYDOWN;
        break;
    case WM_RBUTTONUP:
        last_input_vk = VK_RBUTTON;
        last_transition = WM_KEYUP;
        break;
    case WM_XBUTTONDOWN:
        INFO(GET_XBUTTON_WPARAM(mouseData));
        if (GET_XBUTTON_WPARAM(mouseData) == XBUTTON1)
        {
            last_input_vk = VK_XBUTTON1;
        }
        else if (GET_XBUTTON_WPARAM(mouseData) == XBUTTON2)
        {
            last_input_vk = VK_XBUTTON2;
        }
        last_transition = WM_KEYDOWN;
        break;
    case WM_XBUTTONUP:
        if (GET_XBUTTON_WPARAM(mouseData) == XBUTTON1)
        {
            last_input_vk = VK_XBUTTON1;
        }
        else if (GET_XBUTTON_WPARAM(mouseData) == XBUTTON2)
        {
            last_input_vk = VK_XBUTTON2;
        }
        last_transition = WM_KEYUP;
        break;
    default:
        event_was_button_press_or_release = false;
    }
    return event_was_button_press_or_release;
}
