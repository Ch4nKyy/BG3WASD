#include "BlockHoldInteractMovePatch.hpp"
#include "../Settings.hpp"
#include "../State.hpp"

bool BlockHoldInteractMovePatch::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "0F ?? ?? ?? ?? ?? 81 ?? ?? ?? ?? ?? 0F ?? ?? ?? ?? ?? 83 ?? ?? 0F ?? ?? ?? ?? ?? 48">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("BlockHoldInteractMovePatch #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    DEBUG("BlockHoldInteractMovePatch found: {:X}", AsAddress(addresses[0]));
    return all_found;
}

void BlockHoldInteractMovePatch::Enable()
{
    if (not all_found)
    {
        return;
    }

    if (!handle)
    {
        BlockHoldInteractMovePatchAsm patch;
        patch.ready();
        handle = dku::Hook::AddASMPatch(addresses[0], { 0, 6 }, &patch);
    }
    handle->Enable();
}

void BlockHoldInteractMovePatch::Disable()
{
    if (not all_found)
    {
        return;
    }

    if (!handle)
    {
        return;
    }
    handle->Disable();
}
