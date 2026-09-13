#pragma once

#include "Hooks/Base/CaveHook.hpp"

// Caller: ActiveCharacterChanged
// Target: After a check that the new character is not the same as the old character.
// Thread: GameThread
// Called when: Active character changes or a game is loaded.
// Purpose: Detect when the active character changes, to update camera rotation.
//
// New character is in register R9.
class ActiveCharacterChangedCavehook : public CaveHook
{
public:
    ActiveCharacterChangedCavehook() :
        CaveHook({ search_pattern<"4C 89 4E 08 48 8B 19 48 89 7C 24 ?? E8">() }, { -5 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;
    static void Func(int64_t invalid, int64_t invalid2, int64_t invalid3, int64_t character);

    static ActiveCharacterChangedCavehook& Get()
    {
        static ActiveCharacterChangedCavehook instance;
        return instance;
    }
};
