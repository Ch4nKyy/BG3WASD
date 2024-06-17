#include "SetCursorRotateHook.hpp"
#include "../Settings.hpp"
#include "../State.hpp"

bool SetCursorRotateHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "E8 ?? ?? ?? FF EB 05 E8 ?? ?? ?? FF 4C 8B 4D 7F">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("SetCursorRotateHook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void SetCursorRotateHook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<5>(address, OverrideFunc);
        DEBUG("Hooked SetCursorRotateHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

// Called in GameThread, when Camera Rotate down
void SetCursorRotateHook::OverrideFunc(int64_t a1, int a2)
{
    if (*Settings::GetSingleton()->enable_improved_mouselook)
    {
        auto* state = State::GetSingleton();

        state->SetInternalIsRotating(true);

        int x, y;
        SDL_GetMouseState(&x, &y);
        state->cursor_position_to_restore.x = x;
        state->cursor_position_to_restore.y = y;
        state->HideCursor(true);
    }

    return OriginalFunc(a1, a2);
}
