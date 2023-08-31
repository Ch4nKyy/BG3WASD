#pragma once

#include <string>

class InputFaker
{
public:
    static void SendKeyDownAndUp(std::string keyname);
    static void SendKey(std::string keyname, bool down);
    static void SendMouseMotion(int xrel, int yrel);
private:
    static void SendKeyMouse(std::string keyname, bool down);
    static void SendKeyKeyboard(std::string keyname, bool down, std::vector<std::string> modifiers);
};
