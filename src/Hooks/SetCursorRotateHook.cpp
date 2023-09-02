#include "SetCursorRotateHook.hpp"
#include "../InputFaker.hpp"
#include "../Settings.hpp"
#include "../State.hpp"

bool SetCursorRotateHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "E8 ?? ?? ?? ?? EB ?? E8 ?? ?? ?? ?? 45 ?? ?? ?? 75 ?? 40 ?? ?? 40 ?? ?? ?? ?? ?? ?? 44 ?? "
        "?? ?? 40 ?? ?? 0F ?? ?? ?? ?? ?? E9 ?? ?? ?? ?? F3 ?? ?? ?? ?? ?? ?? ?? 0F ?? ?? 76">()) };
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
        INFO("Hooked SetCursorRotateHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

// Called in GameThread, when Camera Rotate down
void SetCursorRotateHook::OverrideFunc(int64_t a1, int a2)
{
    if (!*Settings::GetSingleton()->enable_improved_mouselook)
    {
        return OriginalFunc(a1, a2);
    }

    auto* state = State::GetSingleton();
    if (!state->set_is_rotating_was_faked)
    {
        state->mouselook_toggled = false;
    }
    state->set_is_rotating_was_faked = false;

    state->is_rotating_changed = true;
    state->SetInternalIsRotating(true);
    GetCursorPos(&state->cursor_position_to_restore);
    if (Settings::GetSingleton()->enable_rightclick_mouselook_fix)
    {
        state->rotate_start_time = SDL_GetTicks();
    }
    else
    {
        InputFaker::SendMouseMotion(0, 0);
    }
    return OriginalFunc(a1, a2);
}
