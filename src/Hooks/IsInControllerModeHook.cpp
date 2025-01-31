#include "IsInControllerModeHook.hpp"

void IsInControllerModeHook::EnableSpecifically(uintptr_t address_incl_offset)
{
    int32_t offset = *reinterpret_cast<int32_t*>(address_incl_offset + 2);
    is_in_controller_mode = reinterpret_cast<bool*>(address_incl_offset + 7 + offset);
}

bool IsInControllerModeHook::Read()
{
    if (is_in_controller_mode == nullptr)
    {
        return false;
    }

    return *is_in_controller_mode;
}
