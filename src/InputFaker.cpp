#include "InputFaker.hpp"
#include "CommandId.hpp"
#include "SDL.h"
#include "Structs/CommandHandler.hpp"
#include "VirtualKeyMap.hpp"

using enum CommandId;

int InputFaker::SendCommand(CommandId command_id, bool pressed, float value1, float value2)
{
    Command command{
        .id = static_cast<uint32_t>(command_id),
        .device_id = 0,
        .new_value1 = value1,
        .new_value2 = value2,
        .new_pressed = pressed,
    };
    return SendCommand(command);
}

int InputFaker::SendCommand(Command command)
{
    CommandHandler* handler = GetCommandHandler(static_cast<CommandId>(command.id));

    if (!handler)
    {
        return 0;
    }

    WORD return_value = 0;

    handler->HandleCommand(&return_value, &command);

    return return_value;
}

void InputFaker::QueueCommand(CommandId command_id, bool pressed, float value1, float value2)
{
    pending_commands.push(Command{
        .id = static_cast<uint32_t>(command_id),
        .device_id = 0,
        .new_value1 = value1,
        .new_value2 = value2,
        .new_pressed = pressed,
    });
}

void InputFaker::ProcessQueuedCommands()
{
    while (!pending_commands.empty())
    {
        Command command = pending_commands.front();
        pending_commands.pop();
        SendCommand(static_cast<CommandId>(command.id), command.new_pressed, command.new_value1,
            command.new_value2);
    }
}

// Get the correct command handler for a given command id.
// If you want to use new commands, you have to add them here.
// Note that there are more than just these two handlers. Some commands might need hooking a new one.
CommandHandler* InputFaker::GetCommandHandler(CommandId command_id)
{
    switch (command_id)
    {
    case CameraToggleMouseRotate:
    case CameraBackward:
    case CameraForward:
    case CameraLeft:
    case CameraRight:
    case CameraRotate:
        return camera_command_handler;

    case CharacterMoveForward:
    case CharacterMoveBackward:
    case CharacterMoveLeft:
    case CharacterMoveRight:
    case HighlightCharacters:
    case ActionCancel:
    case ContextMenu:
        return character_command_handler;

    default:
        return nullptr;
    }
}

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
        SendKey_Mouse(main_key, down);
    }
    else if (prefix == "key")
    {
        SendKey_Keyboard(main_key, down, modifiers);
    }
}

void InputFaker::SendKey_Mouse(std::string keyname, bool down)
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

void InputFaker::SendKey_Keyboard(std::string keyname, bool down,
    std::vector<std::string> modifiers)
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
