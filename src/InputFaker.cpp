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
    auto split = dku::string::split(keyname, ":"sv);
    auto prefix = split[0];
    if (prefix == "mouse")
    {
        SendKeyMouseOnly(keyname, down);
    }
    else if (prefix == "key")
    {
        SendKeyKeyboardOnly(keyname, down);
    }
}

void InputFaker::SendKeyMouseOnly(std::string keyname, bool down)
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

void InputFaker::SendKeyKeyboardOnly(std::string keyname, bool down)
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
    event.key.keysym.mod = 0;  // TODO might support this in the future
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
