#include "SDL_GetWindowGrabHook.hpp"
#include "../State.hpp"
#include "SDL.h"

bool SDL_GetWindowGrabHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "E8 ?? ?? ?? ?? 3B ?? 74 ?? 48 ?? ?? ?? 8B ?? E8 ?? ?? ?? ?? 48 ?? ?? ?? ?? 74 ?? 8B ?? ?? "
        "2B ?? ?? 8B ?? ?? 44 ?? ?? 2B ?? ?? 44 ?? ?? E9 ?? ?? ?? ?? 48 ?? ?? ?? 4C ?? ?? ?? ?? "
        "66">()) };
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
        OriginalFunc = dku::Hook::write_call<5>(address, OverrideFunc);
        INFO("Hooked SDL_GetWindowGrabHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

int64_t SDL_GetWindowGrabHook::OverrideFunc(int64_t a1)
{
    // TODO hook is only used for this. rename class accordingly?
    State::GetSingleton()->sdl_window = reinterpret_cast<SDL_Window*>(a1);

    return OriginalFunc(a1);
}
