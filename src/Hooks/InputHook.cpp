#include "InputHook.hpp"
#include "../ToggleRequest.hpp"
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
    is_shift_down = (GetAsyncKeyState(VK_SHIFT) & 0x8000);
    is_ctrl_down = (GetAsyncKeyState(VK_CONTROL) & 0x8000);
    is_alt_down = (GetAsyncKeyState(VK_MENU) & 0x8000);
    auto* state = State::GetSingleton();
    AutoRun(state);
    ToggleMovementMode(state);
    WalkOrSprint(state);
    ReloadConfig();
    MouseLeftDown();
    ToggleMouselook();
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
        bool combo_includes_shift = false;
        bool combo_includes_ctrl = false;
        bool combo_includes_alt = false;
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
            combo_includes_shift |= (vk == VK_SHIFT);
            combo_includes_ctrl |= (vk == VK_CONTROL);
            combo_includes_alt |= (vk == VK_MENU);
        }
        if (!combo_includes_shift && is_shift_down || !combo_includes_ctrl && is_ctrl_down ||
            !combo_includes_alt && is_alt_down)
        {
            changed = false;
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
    if (DidCommandChange(TOGGLE_AUTOFORWARD, WM_KEYDOWN) && state->IsWasdCharacterMovement())
    {
        state->autoforward_toggled = !state->autoforward_toggled;
        return;
    }

    if (DidCommandChange(FORWARD, WM_KEYDOWN) || DidCommandChange(BACKWARD, WM_KEYDOWN) ||
        DidCommandChange(TOGGLE_MOVEMENT_MODE, WM_KEYDOWN))
    {
        state->autoforward_toggled = false;
        return;
    }
}

void InputHook::ToggleMovementMode(State* state)
{
    if (DidCommandChange(TOGGLE_MOVEMENT_MODE, WM_KEYDOWN))
    {
        state->SetIsWasdCharacterMovement(!state->IsWasdCharacterMovement());
        return;
    }
}

void InputHook::WalkOrSprint(State* state)
{
    if (DidCommandChange(TOGGLE_WALKSPEED, WM_KEYDOWN) && state->IsWasdCharacterMovement())
    {
        state->walking_toggled = !state->walking_toggled;
        return;
    }

    if (DidCommandChange(HOLD_WALKSPEED, WM_KEYDOWN) && state->IsWasdCharacterMovement())
    {
        state->walking_held = true;
        return;
    }

    if (DidCommandChange(HOLD_WALKSPEED, WM_KEYUP) && state->IsWasdCharacterMovement())
    {
        state->walking_held = false;
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

void InputHook::MouseLeftDown()
{
    auto* state = State::GetSingleton();
    if (DidCommandChange(MOUSE_LEFT_DOWN, WM_KEYDOWN))
    {
        state->is_mouseleft_pressed = true;
    }
    if (DidCommandChange(MOUSE_LEFT_DOWN, WM_KEYUP))
    {
        state->is_mouseleft_pressed = false;
    }
}

void InputHook::ToggleMouselook()
{
    // TODO ToggleMouselook
    // auto* state = State::GetSingleton();
    // if (DidCommandChange(TOGGLE_MOUSELOOK, WM_KEYDOWN))
    // {
    //     if (state->mouselook_toggled)
    //     {
    //         state->mouselook_request = ToggleRequest::OFF;
    //     }
    //     else
    //     {
    //         state->mouselook_request = ToggleRequest::ON;
    //     }
    // }
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
