#include "LoadInputConfigFunctionRef.hpp"

void LoadInputConfigFunctionRef::EnableSpecifically(intptr_t address_incl_offset)
{
    function = reinterpret_cast<Fn>(address_incl_offset);
}
