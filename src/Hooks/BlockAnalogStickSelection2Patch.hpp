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

// On game start, set Analog Stick Selection to disabled (2).
class BlockAnalogStickSelection2Patch : public PatchHook
{
public:
    BlockAnalogStickSelection2Patch() :
        PatchHook({ search_pattern<"48 ?? ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 ?? "
                                                        "?? ?? ?? ?? ?? 41 ?? ?? ?? 0E 00 00">() },
            { 0 }, std::source_location::current().function_name()) {};
    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static BlockAnalogStickSelection2Patch& Get()
    {
        static BlockAnalogStickSelection2Patch instance;
        return instance;
    }
};
