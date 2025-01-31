#include "SDL_GetWindowGrabHook.hpp"
#include "SDL.h"
#include "State.hpp"

void SDL_GetWindowGrabHook::EnableSpecifically(uintptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<6>(address_incl_offset, OverrideFunc);
}

// Called in MainThread, every frame that has a mouse motion event
int64_t SDL_GetWindowGrabHook::OverrideFunc(int64_t a1)
{
    State::GetSingleton()->sdl_window = reinterpret_cast<SDL_Window*>(a1);

    return OriginalFunc(a1);
}
