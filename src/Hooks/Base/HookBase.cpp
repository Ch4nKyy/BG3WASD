#include "Hooks/Base/HookBase.hpp"

void HookBase::Enable()
{
    if (not found_all_addresses)
    {
        return;
    }
    for (std::size_t i = 0; i < addresses.size(); ++i)
    {
        EnableSpecifically(addresses[i] + offsets[i]);
        DEBUG("Hooked {} #{}: {:X}", name, i, AsAddress(addresses[i]));
    }
}
