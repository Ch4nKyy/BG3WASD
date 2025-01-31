#pragma once

#include "Hooks/Base/PatchHook.hpp"

struct WASDUnlockPatchAsm : Xbyak::CodeGenerator
{
    WASDUnlockPatchAsm()
    {
        nop();  // 1
        nop();
        nop();
        nop();
        nop();  // 5
        nop();
        nop();
        nop();
        nop();
        nop();  // 10
        nop();
        nop();
        nop();  // 13
    }
};

class WASDUnlockPatch : public PatchHook
{
public:
    WASDUnlockPatch() :
        PatchHook(
            { search_pattern<"44 ?? ?? ?? ?? ?? ?? 0F ?? ?? ?? ?? ?? 49 ?? ?? "
                                                  "?? 8B ?? ?? ?? ?? ?? 39 ?? ?? ?? ?? ?? "
                                                  "0F">(0) },
            { 0 }, std::source_location::current().function_name()) {};
    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static WASDUnlockPatch& Get()
    {
        static WASDUnlockPatch instance;
        return instance;
    }
};
