#pragma once

#include "Hooks/Base/CaveHook.hpp"

struct Command;

// Caller:
// Target:
// Thread: GameThread
// Called when: a character command is handled.
// Purpose: React to or manipulate character commands.
//
// - Cancel interact move, when WASD is pressed.
class HandleCharacterCommandCavehook : public CaveHook
{
public:
    HandleCharacterCommandCavehook() :
        CaveHook({ search_pattern<"48 8B 05 ?? ?? ?? ?? 49 8B F8 48 8B DA 48 8B F1 4C 8B 98 90 00 "
                                  "00 00 4D 85 DB 74 ??">() },
            { 7 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;
    static void Func(uint64_t* handler, WORD* result, Command* command);

    static HandleCharacterCommandCavehook& Get()
    {
        static HandleCharacterCommandCavehook instance;
        return instance;
    }
};
