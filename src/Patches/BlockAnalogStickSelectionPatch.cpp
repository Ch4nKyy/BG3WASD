#include "BlockAnalogStickSelectionPatch.hpp"
#include "../Settings.hpp"
#include "../State.hpp"

bool BlockAnalogStickSelectionPatch::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "89 ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? 41 ?? 5F 5E 5B 5D C3 48 ?? ?? ?? E8">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("BlockAnalogStickSelectionPatch #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    DEBUG("BlockAnalogStickSelectionPatch found: {:X}", AsAddress(addresses[0]));
    return all_found;
}

void BlockAnalogStickSelectionPatch::Activate()
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

void BlockAnalogStickSelectionPatch::Deactivate()
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

void BlockAnalogStickSelectionPatch::Enable()
{
    BlockAnalogStickSelectionPatchAsm patch;
    patch.ready();
    handle = dku::Hook::AddASMPatch(addresses[0], { 0, 6 }, &patch);
}
