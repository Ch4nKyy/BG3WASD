#include "SDL_GetWindowGrabHook.hpp"
#include "../State.hpp"
#include "SDL.h"

bool SDL_GetWindowGrabHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "FF ?? ?? ?? ?? ?? 3B ?? 74 ?? 48 ?? ?? ?? 8B ?? FF">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("SDL_GetWindowGrabHook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void SDL_GetWindowGrabHook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<6>(address, OverrideFunc);
        DEBUG("Hooked SDL_GetWindowGrabHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

// Called in MainThread, every frame that has a mouse motion event
int64_t SDL_GetWindowGrabHook::OverrideFunc(int64_t a1)
{
    State::GetSingleton()->sdl_window = reinterpret_cast<SDL_Window*>(a1);

    return OriginalFunc(a1);
}
