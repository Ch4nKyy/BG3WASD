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

// Caller: UpdateWasdMove2
// Target: Nop a IsInControllerMode check in UpdateWasdMove2.
// Thread: GameThread
// Called when: character movement is allowed, once per frame.
// Purpose: Unlock WASD movement in keyboard mode.
class WASDUnlockPatch : public PatchHook
{
public:
    WASDUnlockPatch() :
        PatchHook(
            { search_pattern<
                "80 ?? ?? ?? ?? ?? ?? 0F ?? ?? ?? ?? ?? 49 ?? ?? ?? 8B ?? ?? ?? ?? ?? 39">(0) },
            { 0 }, std::source_location::current().function_name()) {};
    void EnableSpecifically(intptr_t address_incl_offset) override;

    static WASDUnlockPatch& Get()
    {
        static WASDUnlockPatch instance;
        return instance;
    }
};
