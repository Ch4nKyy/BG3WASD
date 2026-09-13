#include "IsInControllerModeGlobalRef.hpp"

void IsInControllerModeGlobalRef::EnableSpecifically(intptr_t address_incl_offset)
{
    int32_t displacement = *reinterpret_cast<int32_t*>(address_incl_offset + 2);
    value = reinterpret_cast<bool*>(address_incl_offset + 7 + displacement);
}
