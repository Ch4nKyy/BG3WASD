#include "MessageBox.hpp"

void MessageBox::Show(std::string error)
{
    std::thread([](std::string error) { Show_Impl(error); }, error).detach();
}

void MessageBox::Show_Impl(std::string error)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "BG3WASD", error.c_str(), NULL);
}
