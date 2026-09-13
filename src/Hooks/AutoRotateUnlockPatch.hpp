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

// Caller: UpdateCameraRotation
// Target: auto-rotation section
// Thread: GameThread
// Called when: the camera rotates or moves.
// Purpose: Unlock auto-rotate functionality for WASD movement.
//
// In older versions, the vanilla auto-rotate feature worked for WASD, but after some update, it was blocked.
// This patch unlocks it.
class AutoRotateUnlockPatch : public PatchHook
{
public:
    AutoRotateUnlockPatch() :
        PatchHook(
            { search_pattern<
                "44 ?? ?? ?? ?? ?? ?? 0F ?? ?? ?? ?? ?? 48 ?? ?? E8 ?? ?? ?? ?? 48 ?? ?? 0F">(0) },
            { 0 }, std::source_location::current().function_name()) {};
    void EnableSpecifically(intptr_t address_incl_offset) override;

    static AutoRotateUnlockPatch& Get()
    {
        static AutoRotateUnlockPatch instance;
        return instance;
    }
};
