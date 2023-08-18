#include "FTBStartHook.hpp"
#include "../State.hpp"

bool FTBStartHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "8D ?? ?? ?? ?? ?? 48 ?? ?? ?? 49 ?? ?? E8 ?? ?? ?? ?? 4D ?? ?? 4C ?? ?? ?? 48 ?? ?? 49 ?? "
        "?? E8 ?? ?? ?? ?? 48">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            WARN("FTBStartHook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void FTBStartHook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<5>(address + 13, OverrideFunc);
        INFO("Hooked FTBStartHook #{}: {:X}", i, AsAddress(address + 13));
        ++i;
    }
}

int64_t FTBStartHook::OverrideFunc(int64_t a1, int64_t a2, char* a3)
{
    int64_t ret = OriginalFunc(a1, a2, a3);

    auto* state = State::GetSingleton();
    state->is_wasd_character_movement = false;

    return ret;
}