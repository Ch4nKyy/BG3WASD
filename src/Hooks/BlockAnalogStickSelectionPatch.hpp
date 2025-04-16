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
    }
};

// When changing Analog Stick Selection in the menu, ignore it.
class BlockAnalogStickSelectionPatch : public PatchHook
{
public:
    BlockAnalogStickSelectionPatch() :
        PatchHook({ search_pattern<
                      "89 90 70 0E 00 00">() },
            { 0 }, std::source_location::current().function_name()) {};
    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static BlockAnalogStickSelectionPatch& Get()
    {
        static BlockAnalogStickSelectionPatch instance;
        return instance;
    }
};
