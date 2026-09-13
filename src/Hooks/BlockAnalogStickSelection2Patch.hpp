#pragma once

#include "Hooks/Base/PatchHook.hpp"

struct BlockAnalogStickSelection2PatchAsm : Xbyak::CodeGenerator
{
    BlockAnalogStickSelection2PatchAsm()
    {
        mov(eax, 2);
        nop();
        nop();
        nop();
        nop();
        nop();
        nop();
        nop();
    }
};

// Caller: InitSettings
// Target: GetValueForKey
// Thread: GameThread
// Called when: game settings are initialized.
// Purpose: On game start, set Analog Stick Selection to disabled (2).
class BlockAnalogStickSelection2Patch : public PatchHook
{
public:
    BlockAnalogStickSelection2Patch() :
        PatchHook({ search_pattern<"48 8B 8B 60 01 00 00 E8 ?? ?? ?? ?? 48 8B 15 ?? ?? ?? ?? 41 89 "
                                   "82 70 0E 00 00">() },
            { 0 }, std::source_location::current().function_name()) {};
    void EnableSpecifically(intptr_t address_incl_offset) override;

    static BlockAnalogStickSelection2Patch& Get()
    {
        static BlockAnalogStickSelection2Patch instance;
        return instance;
    }
};
