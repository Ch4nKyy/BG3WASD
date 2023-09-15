#include "BlockInteractMovePatch.hpp"
#include "../Settings.hpp"
#include "../State.hpp"

bool BlockInteractMovePatch::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "74 ?? 48 ?? ?? ?? ?? 48 ?? ?? E8 ?? ?? ?? ?? 48 ?? ?? 66">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("BlockInteractMovePatch #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    DEBUG("BlockInteractMovePatch found: {:X}", AsAddress(addresses[0]));
    return all_found;
}

void BlockInteractMovePatch::Activate()
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

void BlockInteractMovePatch::Deactivate()
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

void BlockInteractMovePatch::Enable()
{
    BlockInteractMovePatchAsm patch;
    patch.ready();
    handle = dku::Hook::AddASMPatch(addresses[0], { 0, 2 }, &patch);
}
