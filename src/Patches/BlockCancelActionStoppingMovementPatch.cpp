#include "BlockCancelActionStoppingMovementPatch.hpp"
#include "Settings.hpp"
#include "State.hpp"

bool BlockCancelActionStoppingMovementPatch::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "83 ?? ?? 0F ?? ?? ?? ?? ?? 48 ?? ?? ?? 48 ?? ?? 74 ?? F7">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("BlockCancelActionStoppingMovementPatch #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    DEBUG("BlockCancelActionStoppingMovementPatch found: {:X}", AsAddress(addresses[0]));
    return all_found;
}

void BlockCancelActionStoppingMovementPatch::Activate()
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

void BlockCancelActionStoppingMovementPatch::Enable()
{
    BlockCancelActionStoppingMovementPatchAsm patch;
    patch.ready();
    handle = dku::Hook::AddASMPatch(addresses[0], { 0, 3 }, &patch);
}

void BlockCancelActionStoppingMovementPatch::Deactivate()
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
