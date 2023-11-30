#include "BlockAnalogStickSelection2Patch.hpp"
#include "../Settings.hpp"
#include "../State.hpp"

bool BlockAnalogStickSelection2Patch::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "48 ?? ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? 41 ?? ?? 60 0E 00 00">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("BlockAnalogStickSelection2Patch #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    DEBUG("BlockAnalogStickSelection2Patch found: {:X}", AsAddress(addresses[0]));
    return all_found;
}

void BlockAnalogStickSelection2Patch::Activate()
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

void BlockAnalogStickSelection2Patch::Deactivate()
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

void BlockAnalogStickSelection2Patch::Enable()
{
    BlockAnalogStickSelection2PatchAsm patch;
    patch.ready();
    handle = dku::Hook::AddASMPatch(addresses[0], { 0, 12 }, &patch);
}
