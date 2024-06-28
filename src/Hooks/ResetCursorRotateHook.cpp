#include "ResetCursorRotateHook.hpp"
#include "Settings.hpp"
#include "State.hpp"

bool ResetCursorRotateHook::Prepare()
{
    std::array<uintptr_t, 3> address_array = {
        AsAddress(
            dku::Hook::Assembly::search_pattern<"E8 ?? ?? ?? FF 4C 8B 4D 7F 41">()),
        AsAddress(dku::Hook::Assembly::search_pattern<
            "E8 ?? ?? ?? FF 48 8B 45 6F 44">()),
        AsAddress(dku::Hook::Assembly::search_pattern<
            "E8 ?? ?? ?? FF 4C 8B ?? ?? 4D 85 ?? 74 ?? 4C 8B">())
    };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("ResetCursorRotateHook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void ResetCursorRotateHook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;

    std::array<uintptr_t, 3> offsets_array = { 0, 0, 0 };

    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<5>(address + offsets_array[i], OverrideFunc);
        DEBUG("Hooked ResetCursorRotateHook #{}: {:X}", i, AsAddress(address + offsets_array[i]));
        ++i;
    }
}

// Called in GameThread, when Camera Rotate key is up or forced by game, e.g. dialog or UI.
void ResetCursorRotateHook::OverrideFunc(int64_t a1, int a2)
{
    if (*Settings::GetSingleton()->enable_improved_mouselook)
    {
        auto* state = State::GetSingleton();
        if (!state->set_is_rotating_was_faked)
        {
            state->mouselook_toggled = false;
        }
        state->set_is_rotating_was_faked = false;

        //  Skip on game launch / first load.
        if (state->cursor_position_to_restore.x != -1)
        {
            state->SetInternalIsRotating(false);
            state->HideCursor(false);
        }
    }

    return OriginalFunc(a1, a2);
}
