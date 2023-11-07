#pragma once

#include "SDL.h"
#include <string>

class MessageBox
{
public:
    // Show SDL message box in own thread so it does not block the game.
    // Blocking the game lead to crashes for some people.
    static void Show(std::string error);

private:
    static void Show_Impl(std::string error);
};
