#pragma once

#include "Hooks/Base/CaveHook.hpp"

// Caller: UpdateInteractMove
// Target: Top of the caller
// Thread: GameThread
// Called when: interact moving, once per frame.
// Purpose: Activates BlockCancelActionStoppingMovementPatch.
class InsideUpdateInteractMoveCavehook : public CaveHook
{
public:
    InsideUpdateInteractMoveCavehook() :
        CaveHook({ search_pattern<"4C 8B 89 80 00 00 00 4C ?? ?? 30 01 00 00">() }, { 0 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;
    static void __cdecl Func();

    static InsideUpdateInteractMoveCavehook& Get()
    {
        static InsideUpdateInteractMoveCavehook instance;
        return instance;
    }
};
