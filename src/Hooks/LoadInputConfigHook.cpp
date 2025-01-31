#include "LoadInputConfigHook.hpp"

void LoadInputConfigHook::EnableSpecifically(uintptr_t address_incl_offset)
{
    OriginalFunc = reinterpret_cast<FunctionSignature>(address_incl_offset);
}
