#pragma once

#include "Structs/Command.hpp"

enum CommandId;
class CommandHandler;

// Can send fake input.
//
// Input is possible on 2 levels.
// 1. Send a key down or up event to the low level SDL input system.
// 2. Send a command directly to the command handlers.
//
// The second is possible because we can capture the pointer to the different CommandHandlers, and then let them call
// the HandleCommand function directly with arbitrary command data.
// Thus, low level input handling is bypassed, but most validation and forwarding to the correct objects is still done
// by the game.
// Because the command is directly sent to the command handler, some things are possible that a user couldn't do.
// E.g. a user can't press CameraRotate while the cursor is hovering over UI, because the UI eats the input.
class InputFaker
{
public:
    // Send two key events, one pressed and one released.
    static void SendKeyDownAndUp(std::string keyname);

    // Send key event, either pressed or released.
    static void SendKey(std::string keyname, bool down);

    // Send relative mouse motion.
    static void SendMouseMotion(int xrel, int yrel);

    // Send a command directly to the command handler, bypassing the low level key input system.
    //
    // Inside this function, the command ids have to be mapped to the correct command handlers.
    // So if a new command does nothing or crashes, check the mapping.
    //
    // If this call still crashes, try QueueCommand.
    static int SendCommand(CommandId command_id, bool pressed, float value1 = 0.0f,
        float value2 = 0.0f);

    static int SendCommand(Command command);

    // SendCommand() must not be called from within certain hooks that would cause recursion or register pollution.
    // In that case, call QueueCommand() instead.
    // In TickInputSystemHook, we call ProcessQueuedCommands() to send the queued commands safely.
    static void QueueCommand(CommandId command_id, bool pressed, float value1 = 0.0f,
        float value2 = 0.0f);

    static void ProcessQueuedCommands();

    // We capture this handler so we can use it to send commands directly to the character command handler.
    static inline CommandHandler* character_command_handler = nullptr;

    // We capture this handler so we can use it to send commands directly to the camera command handler.
    //
    // In contrast to character_command_handler, we cannot capture camera_command_handler directly,
    // because there are multiple camera command handlers that call HandleCameraCommand and we need a specific one.
    // Fortunately, only the correct one calls all the way down to ExecuteCameraCommand, so in there we can
    // promote the candidate to the actual camera_command_handler.
    static inline CommandHandler* camera_command_handler = nullptr;

    // See camera_command_handler.
    static inline CommandHandler* camera_command_handler_candidate = nullptr;

private:
    static void SendKey_Mouse(std::string keyname, bool down);
    static void SendKey_Keyboard(std::string keyname, bool down,
        std::vector<std::string> modifiers);
    static CommandHandler* GetCommandHandler(CommandId command_id);
    static inline std::queue<Command> pending_commands;
};
