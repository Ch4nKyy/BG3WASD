#include "InsideUpdateInteractMoveHook.hpp"
#include "../Settings.hpp"
#include "../State.hpp"

bool InsideUpdateInteractMoveHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "E8 ?? ?? ?? ?? 8B ?? ?? ?? ?? ?? 48 ?? ?? E8 ?? ?? ?? ?? 48 ?? ?? 48 ?? ?? 74 ?? 80">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("InsideUpdateInteractMoveHook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void InsideUpdateInteractMoveHook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<5>(address, OverrideFunc);
        DEBUG("Hooked InsideUpdateInteractMoveHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

int64_t InsideUpdateInteractMoveHook::OverrideFunc(int64_t a1)
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    state->currently_interact_moving = true;

    return OriginalFunc(a1);
}
