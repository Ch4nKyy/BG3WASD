#include "DecideMoveUpdaterHook.hpp"
#include "../Settings.hpp"
#include "../State.hpp"

bool DecideMoveUpdaterHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(
        dku::Hook::Assembly::search_pattern<"E8 ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 "
                                            "?? ?? ?? 48 ?? ?? ?? ?? 48 ?? ?? ?? ?? 48 ?? ?? E8 "
                                            "?? ?? ?? ?? 84">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("DecideMoveUpdaterHook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void DecideMoveUpdaterHook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<5>(address, OverrideFunc);
        DEBUG("Hooked DecideMoveUpdaterHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

int64_t DecideMoveUpdaterHook::OverrideFunc(int64_t a1, int64_t a2)
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    // If we are, then this is immediately overwritten in InsideUpdateInteractMove
    state->SetCurrentlyInteractMoving(false);

    return OriginalFunc(a1, a2);
}
