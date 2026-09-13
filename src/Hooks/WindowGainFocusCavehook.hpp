#pragma once

#include "Hooks/Base/CaveHook.hpp"

// Caller: SetWindowActive
// Target: Inside branch where active = true.
// Thread: GameThread
// Called when: the game window gains focus (e.g. after alt-tabbing back into the game)
// Purpose: Detect when the game window gains focus, to update camera rotation.
class WindowGainFocusCavehook : public CaveHook
{
public:
    WindowGainFocusCavehook() :
        CaveHook({ search_pattern<"8B 82 E0 01 00 00 89 82 E4 01 00 00">() }, { 0 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static void Func();

    static WindowGainFocusCavehook& Get()
    {
        static WindowGainFocusCavehook instance;
        return instance;
    }
};
