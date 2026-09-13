#pragma once

#include "Hooks/Base/PatchHook.hpp"

struct BlockCancelActionStoppingMovementPatchAsm : Xbyak::CodeGenerator
{
    BlockCancelActionStoppingMovementPatchAsm() { test(rsp, rsp); }
};

// Caller: Unknown
// Target: Section where command ID is checked for ActionCancel.
// Thread: GameThread
// Called when: any command is processed.
// Purpose: Block ActionCancel from stopping direct character movement.
//
// This is done so movement does not stutter, when rotating with rightclick AND
// Cancel does not block Context Menu, making using Context Menu out of movement easier.
class BlockCancelActionStoppingMovementPatch : public PatchHook
{
public:
    BlockCancelActionStoppingMovementPatch() :
        PatchHook({ search_pattern<
                      "83 ?? ?? 0F ?? ?? ?? ?? ?? 48 ?? ?? ?? 48 ?? ?? 74 ?? F7">() },
            { 0 }, std::source_location::current().function_name()) {};
    void EnableSpecifically(intptr_t address_incl_offset) override;

    static BlockCancelActionStoppingMovementPatch& Get()
    {
        static BlockCancelActionStoppingMovementPatch instance;
        return instance;
    }
};
