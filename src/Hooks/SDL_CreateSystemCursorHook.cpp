#include "SDL_CreateSystemCursorHook.hpp"

void SDL_CreateSystemCursorHook::EnableSpecifically(intptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<6>(address_incl_offset, OverrideFunc);
}

int64_t SDL_CreateSystemCursorHook::OverrideFunc(QWORD a1) { return OriginalFunc(3); }
