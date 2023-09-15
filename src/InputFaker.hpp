#pragma once

#include <string>

enum GameCommand;

class InputFaker
{
public:
    static void SendKeyDownAndUp(std::string keyname);
    static void SendKey(std::string keyname, bool down);
    static void SendMouseMotion(int xrel, int yrel);
    static int SendCommand(GameCommand command_id, bool down);

    static inline int64_t* game_input_manager;

private:
    static void SendKeyMouse(std::string keyname, bool down);
    static void SendKeyKeyboard(std::string keyname, bool down, std::vector<std::string> modifiers);
};
