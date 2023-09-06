#pragma once

#include <map>
#include <string>
#include <winuser.rh>

#include "DKUtil/Config.hpp"
#include "SDL.h"

using namespace DKUtil::Alias;

enum Command
{
    TOGGLE_WALKSPEED,
    HOLD_WALKSPEED,
    TOGGLE_AUTOFORWARD,
    TOGGLE_MOVEMENT_MODE,
    RELOAD_CONFIG,
    FORWARD,
    BACKWARD,
    MOUSE_LEFT_DOWN,
    TOGGLE_MOUSELOOK
};

class VirtualKeyMap
{
public:
    static int GetVkByName(const std::string name);
    static int GetScancodeByName(const std::string name);
    static int GetKeycodeByName(const std::string name);
    static std::vector<std::vector<std::uint32_t>> GetVkCombosOfCommand(Command command);
    static void UpdateVkCombosOfCommandMap();
    static bool VkIsValid(std::string keyname);

private:
    static void AddKeyComboForCommand(Command command, std::vector<std::string> setting);
    static inline std::map<Command, std::vector<std::vector<std::uint32_t>>> vkcombos_of_command;
    // clang-format off

    /*
     Names are adapted to match Larian's names!
    */
    static inline std::map<std::string, std::vector<int>> VIRTUAL_KEY_MAP{
        { "mouse:left", { VK_LBUTTON, NULL, SDL_BUTTON_LEFT } },
        { "mouse:right", { VK_RBUTTON, NULL, SDL_BUTTON_RIGHT } },
        { "mouse:middle", { VK_MBUTTON, NULL, SDL_BUTTON_MIDDLE } },
        { "mouse:x1", { VK_XBUTTON1, NULL, SDL_BUTTON_X1 } },
        { "mouse:x2", { VK_XBUTTON2, NULL, SDL_BUTTON_X2 } },
        { "key:backspace", { VK_BACK, SDL_SCANCODE_BACKSPACE, SDLK_BACKSPACE } },
        { "key:tab", { VK_TAB, SDL_SCANCODE_TAB, SDLK_TAB } },
        { "key:clear", { VK_CLEAR, SDL_SCANCODE_CLEAR, SDLK_CLEAR } },
        { "key:return", { VK_RETURN, SDL_SCANCODE_RETURN, SDLK_RETURN } },
        { "shift", { VK_SHIFT, NULL, NULL } },
        { "ctrl", { VK_CONTROL, NULL, NULL } },
        { "alt", { VK_MENU, NULL, NULL } },
        { "key:pause", { VK_PAUSE, SDL_SCANCODE_PAUSE, SDLK_PAUSE } },
        { "key:capslock", { VK_CAPITAL, SDL_SCANCODE_CAPSLOCK, SDLK_CAPSLOCK } },
        { "key:escape", { VK_ESCAPE, SDL_SCANCODE_ESCAPE, SDLK_ESCAPE } },
        { "key:modechange", { VK_MODECHANGE, SDL_SCANCODE_MODE, SDLK_MODE } },
        { "key:space", { VK_SPACE, SDL_SCANCODE_SPACE, SDLK_SPACE } },
        { "key:prior", { VK_PRIOR, SDL_SCANCODE_PRIOR, SDLK_PRIOR } },
        { "key:end", { VK_END, SDL_SCANCODE_END, SDLK_END } },
        { "key:home", { VK_HOME, SDL_SCANCODE_HOME, SDLK_HOME } },
        { "key:left", { VK_LEFT, SDL_SCANCODE_LEFT, SDLK_LEFT } },
        { "key:up", { VK_UP, SDL_SCANCODE_UP, SDLK_UP } },
        { "key:right", { VK_RIGHT, SDL_SCANCODE_RIGHT, SDLK_RIGHT } },
        { "key:down", { VK_DOWN, SDL_SCANCODE_DOWN, SDLK_DOWN } },
        { "key:select", { VK_SELECT, SDL_SCANCODE_SELECT, SDLK_SELECT } },
        { "key:printscreen", { VK_PRINT, SDL_SCANCODE_PRINTSCREEN, SDLK_PRINTSCREEN } },
        { "key:execute", { VK_EXECUTE, SDL_SCANCODE_EXECUTE, SDLK_EXECUTE } },
        { "key:insert", { VK_INSERT, SDL_SCANCODE_INSERT, SDLK_INSERT } },
        { "key:delete", { VK_DELETE, SDL_SCANCODE_DELETE, SDLK_DELETE } },
        { "key:help", { VK_HELP, SDL_SCANCODE_HELP, SDLK_HELP } },
        { "key:lgui", { VK_LWIN, SDL_SCANCODE_LGUI, SDLK_LGUI } },
        { "key:rgui", { VK_RWIN, SDL_SCANCODE_RGUI, SDLK_RGUI } },
        { "key:apps", { VK_APPS, SDL_SCANCODE_APPLICATION, SDLK_APPLICATION } },
        { "key:sleep", { VK_SLEEP, SDL_SCANCODE_SLEEP, SDLK_SLEEP } },
        { "key:kp_0", { VK_NUMPAD0, SDL_SCANCODE_KP_0, SDLK_KP_0 } },
        { "key:kp_1", { VK_NUMPAD1, SDL_SCANCODE_KP_1, SDLK_KP_1 } },
        { "key:kp_2", { VK_NUMPAD2, SDL_SCANCODE_KP_2, SDLK_KP_2 } },
        { "key:kp_3", { VK_NUMPAD3, SDL_SCANCODE_KP_3, SDLK_KP_3 } },
        { "key:kp_4", { VK_NUMPAD4, SDL_SCANCODE_KP_4, SDLK_KP_4 } },
        { "key:kp_5", { VK_NUMPAD5, SDL_SCANCODE_KP_5, SDLK_KP_5 } },
        { "key:kp_6", { VK_NUMPAD6, SDL_SCANCODE_KP_6, SDLK_KP_6 } },
        { "key:kp_7", { VK_NUMPAD7, SDL_SCANCODE_KP_7, SDLK_KP_7 } },
        { "key:kp_8", { VK_NUMPAD8, SDL_SCANCODE_KP_8, SDLK_KP_8 } },
        { "key:kp_9", { VK_NUMPAD9, SDL_SCANCODE_KP_9, SDLK_KP_9 } },
        { "key:kp_multiply", { VK_MULTIPLY, SDL_SCANCODE_KP_MULTIPLY, SDLK_KP_MULTIPLY } },
        { "key:kp_plus", { VK_ADD, SDL_SCANCODE_KP_PLUS, SDLK_KP_PLUS } },
        { "key:kp_minus", { VK_SUBTRACT, SDL_SCANCODE_KP_MINUS, SDLK_KP_MINUS } },
        { "key:kp_period", { VK_DECIMAL, SDL_SCANCODE_KP_DECIMAL, SDLK_KP_DECIMAL } },
        { "key:kp_divide", { VK_DIVIDE, SDL_SCANCODE_KP_DIVIDE, SDLK_KP_DIVIDE } },
        { "key:f1", { VK_F1, SDL_SCANCODE_F1, SDLK_F1 } },
        { "key:f2", { VK_F2, SDL_SCANCODE_F2, SDLK_F2 } },
        { "key:f3", { VK_F3, SDL_SCANCODE_F3, SDLK_F3 } },
        { "key:f4", { VK_F4, SDL_SCANCODE_F4, SDLK_F4 } },
        { "key:f5", { VK_F5, SDL_SCANCODE_F5, SDLK_F5 } },
        { "key:f6", { VK_F6, SDL_SCANCODE_F6, SDLK_F6 } },
        { "key:f7", { VK_F7, SDL_SCANCODE_F7, SDLK_F7 } },
        { "key:f8", { VK_F8, SDL_SCANCODE_F8, SDLK_F8 } },
        { "key:f9", { VK_F9, SDL_SCANCODE_F9, SDLK_F9 } },
        { "key:f10", { VK_F10, SDL_SCANCODE_F10, SDLK_F10 } },
        { "key:f11", { VK_F11, SDL_SCANCODE_F11, SDLK_F11 } },
        { "key:f12", { VK_F12, SDL_SCANCODE_F12, SDLK_F12 } },
        { "key:f13", { VK_F13, SDL_SCANCODE_F13, SDLK_F13 } },
        { "key:f14", { VK_F14, SDL_SCANCODE_F14, SDLK_F14 } },
        { "key:f15", { VK_F15, SDL_SCANCODE_F15, SDLK_F15 } },
        { "key:f16", { VK_F16, SDL_SCANCODE_F16, SDLK_F16 } },
        { "key:f17", { VK_F17, SDL_SCANCODE_F17, SDLK_F17 } },
        { "key:f18", { VK_F18, SDL_SCANCODE_F18, SDLK_F18 } },
        { "key:f19", { VK_F19, SDL_SCANCODE_F19, SDLK_F19 } },
        { "key:f20", { VK_F20, SDL_SCANCODE_F20, SDLK_F20 } },
        { "key:f21", { VK_F21, SDL_SCANCODE_F21, SDLK_F21 } },
        { "key:f22", { VK_F22, SDL_SCANCODE_F22, SDLK_F22 } },
        { "key:f23", { VK_F23, SDL_SCANCODE_F23, SDLK_F23 } },
        { "key:f24", { VK_F24, SDL_SCANCODE_F24, SDLK_F24 } },
        { "key:numlock", { VK_NUMLOCK, SDL_SCANCODE_NUMLOCKCLEAR, SDLK_NUMLOCKCLEAR } },
        { "key:scrolllock", { VK_SCROLL, SDL_SCANCODE_SCROLLLOCK, SDLK_SCROLLLOCK } },
        { "key:lshift", { VK_LSHIFT, SDL_SCANCODE_LSHIFT, SDLK_LSHIFT } },
        { "key:rshift", { VK_RSHIFT, SDL_SCANCODE_RSHIFT, SDLK_RSHIFT } },
        { "key:lctrl", { VK_LCONTROL, SDL_SCANCODE_LCTRL, SDLK_LCTRL } },
        { "key:rctrl", { VK_RCONTROL, SDL_SCANCODE_RCTRL, SDLK_RCTRL } },
        { "key:lalt", { VK_LMENU, SDL_SCANCODE_LALT, SDLK_LALT } },
        { "key:ralt", { VK_RMENU, SDL_SCANCODE_RALT, SDLK_RALT } },
        { "key:audio_mute", { VK_VOLUME_MUTE, SDL_SCANCODE_AUDIOMUTE, SDLK_AUDIOMUTE } },
        { "key:volume_down", { VK_VOLUME_DOWN, SDL_SCANCODE_VOLUMEDOWN, SDLK_VOLUMEDOWN } },
        { "key:volume_up", { VK_VOLUME_UP, SDL_SCANCODE_VOLUMEUP, SDLK_VOLUMEUP } },
        { "key:audio_next", { VK_MEDIA_NEXT_TRACK, SDL_SCANCODE_AUDIONEXT, SDLK_AUDIONEXT } },
        { "key:audio_prev", { VK_MEDIA_PREV_TRACK, SDL_SCANCODE_AUDIOPREV, SDLK_AUDIOPREV } },
        { "key:audio_stop", { VK_MEDIA_STOP, SDL_SCANCODE_AUDIOSTOP, SDLK_AUDIOSTOP } },
        { "key:audio_play", { VK_MEDIA_PLAY_PAUSE, SDL_SCANCODE_AUDIOPLAY, SDLK_AUDIOPLAY } },
        { "key:;", { VK_OEM_1, SDL_SCANCODE_SEMICOLON, SDLK_SEMICOLON } },
        { "key:=", { VK_OEM_PLUS, SDL_SCANCODE_EQUALS, SDLK_EQUALS } },
        { "key:,", { VK_OEM_COMMA, SDL_SCANCODE_COMMA, SDLK_COMMA } },
        { "key:-", { VK_OEM_MINUS, SDL_SCANCODE_MINUS, SDLK_MINUS } },
        { "key:.", { VK_OEM_PERIOD, SDL_SCANCODE_PERIOD, SDLK_PERIOD } },
        { "key:/", { VK_OEM_2, SDL_SCANCODE_SLASH, SDLK_SLASH } },
        { "key:`", { VK_OEM_3, SDL_SCANCODE_GRAVE, SDLK_BACKQUOTE } },
        { "key:[", { VK_OEM_4, SDL_SCANCODE_LEFTBRACKET, SDLK_LEFTBRACKET } },
        { "key:\\", { VK_OEM_5, SDL_SCANCODE_BACKSLASH, SDLK_BACKSLASH } },
        { "key:]", { VK_OEM_6, SDL_SCANCODE_RIGHTBRACKET, SDLK_RIGHTBRACKET } },
        { "key:'", { VK_OEM_7, SDL_SCANCODE_APOSTROPHE, SDLK_QUOTE } },
        { "key:crsel", { VK_CRSEL, SDL_SCANCODE_CRSEL, SDLK_CRSEL } },
        { "key:exsel", { VK_EXSEL, SDL_SCANCODE_EXSEL, SDLK_EXSEL } },
        { "key:pagedown", { VK_NEXT, SDL_SCANCODE_PAGEDOWN, SDLK_PAGEDOWN } },
        { "key:pageup", { VK_PRIOR, SDL_SCANCODE_PAGEUP, SDLK_PAGEUP } }
    };
    // clang-format on
};
