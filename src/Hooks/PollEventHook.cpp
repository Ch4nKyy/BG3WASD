#include "PollEventHook.hpp"
#include "../InputFaker.hpp"
#include "../Settings.hpp"
#include "../State.hpp"
#include "SetVirtualCursorPosHook.hpp"

bool PollEventHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "E8 ?? ?? ?? ?? 85 ?? 0F ?? ?? ?? ?? ?? 0F ?? ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? F3 ?? ?? ?? ?? "
        "?? ?? ?? 0F ?? ?? ?? ?? ?? ?? ?? F3 ?? ?? ?? ?? ?? ?? ?? 4C ?? ?? 48 ?? ?? ?? 48 ?? ?? E8 "
        "?? ?? ?? ?? 84">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("PollEventHook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void PollEventHook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<5>(address, OverrideFunc);
        DEBUG("Hooked PollEventHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

// Called in MainThread, every frame
int64_t PollEventHook::OverrideFunc(int64_t a1)
{
    auto* state = State::GetSingleton();
    return OriginalFunc(a1);
}
