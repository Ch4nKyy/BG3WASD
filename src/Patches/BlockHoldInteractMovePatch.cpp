#include "BlockHoldInteractMovePatch.hpp"
#include "../Settings.hpp"
#include "../State.hpp"

bool BlockHoldInteractMovePatch::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "C7 46 20 00 01 00 00">()) };
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
    BlockHoldInteractMovePatchAsm patch;
    patch.ready();
    handle = dku::Hook::AddASMPatch(addresses[0], { 0, 7 }, &patch);
}

void BlockHoldInteractMovePatch::Activate()
{
    if (not all_found)
    {
        return;
    }

    if (!handle)
    {
        return;
    }
    handle->Enable();
}

void BlockHoldInteractMovePatch::Deactivate()
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
