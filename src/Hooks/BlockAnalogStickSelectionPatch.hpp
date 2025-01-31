#pragma once

#include "Hooks/Base/PatchHook.hpp"

struct BlockAnalogStickSelectionPatchAsm : Xbyak::CodeGenerator
{
    BlockAnalogStickSelectionPatchAsm()
    {
        nop();
        nop();
        nop();
        nop();
        nop();
        nop();
        nop();
    }
};

// When changing Analog Stick Selection in the menu, ignore it.
class BlockAnalogStickSelectionPatch : public PatchHook
{
public:
    BlockAnalogStickSelectionPatch() :
        PatchHook({ search_pattern<
                      "44 89 80 ?? 0E 00 00 48 81 C4 ?? ?? ?? ?? 41 ?? 5F 5E 5B 5D C3">() },
            { 0 }, std::source_location::current().function_name()) {};
    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static BlockAnalogStickSelectionPatch& Get()
    {
        static BlockAnalogStickSelectionPatch instance;
        return instance;
    }
};
