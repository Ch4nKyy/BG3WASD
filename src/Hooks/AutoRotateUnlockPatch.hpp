#pragma once

#include "Hooks/Base/PatchHook.hpp"

struct AutoRotateUnlockPatchAsm : Xbyak::CodeGenerator
{
    AutoRotateUnlockPatchAsm()
    {
        nop();  // 1
        nop();
        nop();
        nop();
        nop();  // 5
        nop();
        nop();  // 7
    }
};

class AutoRotateUnlockPatch : public PatchHook
{
public:
    AutoRotateUnlockPatch() :
        PatchHook(
            { search_pattern<
                "44 ?? ?? ?? ?? ?? ?? 0F ?? ?? ?? ?? ?? 48 ?? ?? E8 ?? ?? ?? ?? 48 ?? ?? 0F">(0) },
            { 0 }, std::source_location::current().function_name()) {};
    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static AutoRotateUnlockPatch& Get()
    {
        static AutoRotateUnlockPatch instance;
        return instance;
    }
};
