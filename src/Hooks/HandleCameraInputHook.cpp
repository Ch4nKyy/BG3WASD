#include "../Settings.hpp"
#include "../State.hpp"
#include "HandleCameraInputHook.hpp"

bool HandleCameraInputHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "E8 ?? ?? ?? ?? 0F ?? ?? 66 ?? ?? 80 ?? ?? 75">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("HandleCameraInputHook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void HandleCameraInputHook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<5>(address, OverrideFunc);
        DEBUG("Hooked HandleCameraInputHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

// Called in GameThread, every frame with camera input, while it is allowed
char* HandleCameraInputHook::OverrideFunc(int64_t a1, char* a2, int64_t a3, int* a4)
{
    auto* state = State::GetSingleton();
    return OriginalFunc(a1, a2, a3, a4);
}
