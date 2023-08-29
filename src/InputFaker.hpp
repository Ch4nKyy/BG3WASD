#pragma once

#include <string>

class InputFaker
{
public:
    static void SendKeyDownAndUp(std::string keyname);
    static void SendKey(std::string keyname, bool down);
    static void SendKeyMouseOnly(std::string keyname, bool down);
    static void SendKeyKeyboardOnly(std::string keyname, bool down);
    static void SendMouseMotion(int xrel, int yrel);
};
