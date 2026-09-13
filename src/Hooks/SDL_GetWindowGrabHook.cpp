#include "SDL_GetWindowGrabHook.hpp"
#include "SDL.h"
#include "State.hpp"

void SDL_GetWindowGrabHook::EnableSpecifically(intptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<6>(address_incl_offset, OverrideFunc);
}

int64_t SDL_GetWindowGrabHook::OverrideFunc(int64_t a1)
{
    State::GetSingleton()->sdl_window = reinterpret_cast<SDL_Window*>(a1);

    return OriginalFunc(a1);
}
