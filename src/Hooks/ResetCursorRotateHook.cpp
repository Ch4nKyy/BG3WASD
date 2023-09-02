#include "ResetCursorRotateHook.hpp"
#include "../InputFaker.hpp"
#include "../Settings.hpp"
#include "../State.hpp"

bool ResetCursorRotateHook::Prepare()
{
    std::array<uintptr_t, 3> address_array = {
        AsAddress(
            dku::Hook::Assembly::search_pattern<"E8 ?? ?? ?? ?? 45 ?? ?? ?? 75 ?? 40 ?? ?? 40 ?? "
                                                "?? ?? ?? ?? ?? 44 ?? ?? ?? 40 ?? ?? 0F ?? "
                                                "?? ?? ?? ?? E9">()),
        AsAddress(dku::Hook::Assembly::search_pattern<
            "E8 ?? ?? ?? ?? E9 ?? ?? ?? ?? 44 ?? ?? ?? ?? E9 ?? ?? ?? ?? 90 9B ?? ?? ?? A5 ?? ?? "
            "?? D1 ?? ?? ?? 41 ?? ?? ?? 02">()),
        AsAddress(dku::Hook::Assembly::search_pattern<
            "BA ?? ?? ?? ?? E8 ?? ?? ?? ?? 4C ?? ?? ?? ?? ?? ?? 4D ?? ?? 74 ?? 4C ?? ?? ?? ?? ?? "
            "?? 4D ?? ?? 74 ?? 8B ?? ?? ?? ?? ?? 99 83 ?? ?? 03 ?? 8B ?? 83 ?? ?? 2B ?? C1">())
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

    std::array<uintptr_t, 3> offsets_array = { 0, 0, 5 };

    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<5>(address + offsets_array[i], OverrideFunc);
        INFO("Hooked ResetCursorRotateHook #{}: {:X}", i, AsAddress(address + offsets_array[i]));
        ++i;
    }
}

// Called in GameThread, when Camera Rotate key is up or forced by game, e.g. dialog or UI.
void ResetCursorRotateHook::OverrideFunc(int64_t a1, int a2)
{
    if (!*Settings::GetSingleton()->enable_improved_mouselook)
    {
        return OriginalFunc(a1, a2);
    }

    auto* state = State::GetSingleton();
    if (state->cursor_position_to_restore.x != -1)
    {
        state->is_rotating_changed = true;
        state->SetInternalIsRotating(false);
        state->frames_to_restore_cursor_pos = 2;
    }
    InputFaker::SendMouseMotion(0, 0);
    return OriginalFunc(a1, a2);
}
