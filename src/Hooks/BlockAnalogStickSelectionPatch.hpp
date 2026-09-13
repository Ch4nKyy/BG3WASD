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

// Caller: ChangeSetting
// Target: AnalogStickSelection section
// Thread: GameThread
// Called when: the player attempts to change the Analog Stick Selection setting in the menu.
// Purpose: Ignore changes to the Analog Stick Selection setting.
class BlockAnalogStickSelectionPatch : public PatchHook
{
public:
    BlockAnalogStickSelectionPatch() :
        PatchHook({ search_pattern<
                      "89 90 70 0E 00 00">() },
            { 0 }, std::source_location::current().function_name()) {};
    void EnableSpecifically(intptr_t address_incl_offset) override;

    static BlockAnalogStickSelectionPatch& Get()
    {
        static BlockAnalogStickSelectionPatch instance;
        return instance;
    }
};
