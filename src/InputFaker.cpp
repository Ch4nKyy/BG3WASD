#include "InputFaker.hpp"
#include "SDL.h"
#include "VirtualKeyMap.hpp"

void InputFaker::SendKeyDownAndUp(std::string keyname)
{
    SendKey(keyname, true);
    SendKey(keyname, false);
}

void InputFaker::SendKey(std::string keyname, bool down)
{
    auto split = dku::string::split(keyname, "+"sv);
    auto main_key = split.back();
    split.pop_back();
    auto modifiers = split;

    auto main_key_split = dku::string::split(main_key, ":"sv);
    auto prefix = main_key_split[0];
    if (prefix == "mouse")
    {
        SendKeyMouse(main_key, down);
    }
    else if (prefix == "key")
    {
        SendKeyKeyboard(main_key, down, modifiers);
    }
}

void InputFaker::SendKeyMouse(std::string keyname, bool down)
{
    SDL_Event event;
    event.button.timestamp = SDL_GetTicks();
    event.button.windowID = 0;
    event.button.state = down;
    if (down)
    {
        event.type = event.button.type = SDL_MOUSEBUTTONDOWN;
    }
    else
    {
        event.type = event.button.type = SDL_MOUSEBUTTONUP;
    }
    event.button.button = VirtualKeyMap::GetKeycodeByName(keyname);
    event.button.clicks = 1;
    SDL_PushEvent(&event);
}

void InputFaker::SendKeyKeyboard(std::string keyname, bool down, std::vector<std::string> modifiers)
{
    SDL_Event event;
    event.key.timestamp = SDL_GetTicks();
    event.key.windowID = 0;
    event.key.repeat = 0;
    event.key.state = down;
    if (down)
    {
        event.type = event.key.type = SDL_KEYDOWN;
    }
    else
    {
        event.type = event.key.type = SDL_KEYUP;
    }
    event.key.keysym.scancode =
        static_cast<SDL_Scancode>(VirtualKeyMap::GetScancodeByName(keyname));
    event.key.keysym.sym = VirtualKeyMap::GetKeycodeByName(keyname);
    event.key.keysym.mod = 0;
    // Unfortunately, modifiers don't seem to be detected this way by the game, so it doesn't work.
    for (auto modifier : modifiers)
    {
        if (modifier == "ctrl")
        {
            event.key.keysym.mod |= KMOD_LCTRL;
        }
        else if (modifier == "alt")
        {
            event.key.keysym.mod |= KMOD_LALT;
        }
        else if (modifier == "shift")
        {
            event.key.keysym.mod |= KMOD_LSHIFT;
        }
    }
    SDL_PushEvent(&event);
}

void InputFaker::SendMouseMotion(int xrel, int yrel)
{
    SDL_Event event;
    event.type = event.motion.type = SDL_MOUSEMOTION;
    event.motion.timestamp = SDL_GetTicks();
    event.motion.windowID = 0;
    event.motion.xrel = xrel;
    event.motion.yrel = yrel;
    SDL_PushEvent(&event);
}
