#include "SDL_GetWindowGrabHook.hpp"
#include "../State.hpp"


bool SDL_GetWindowGrabHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "E8 F1 1F 13 02 3B C7 74 0B 48 8B 4E 08 8B D7 E8 DC 1F 13 02">()) };
    addresses = address_array; // TODO pattern?

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
    State::GetSingleton()->sdl_window_ptr = a1; // TODO hook is only used for this. rename class accordingly?
    return OriginalFunc(a1);
}
