#pragma once

#include "Hooks/Base/PatchHook.hpp"

struct BlockCancelActionStoppingMovementPatchAsm : Xbyak::CodeGenerator
{
    BlockCancelActionStoppingMovementPatchAsm() { test(rsp, rsp); }
};

// Blocks Cancel Action from stopping direct character movement.
// This is done so movement does not stutter, when rotating with rightclick AND
// Cancel does not block Context Menu, making using Context Menu out of movement easier.
class BlockCancelActionStoppingMovementPatch : public PatchHook
{
public:
    BlockCancelActionStoppingMovementPatch() :
        PatchHook({ search_pattern<
                      "83 ?? ?? 0F ?? ?? ?? ?? ?? 48 ?? ?? ?? 48 ?? ?? 74 ?? F7">() },
            { 0 }, std::source_location::current().function_name()) {};
    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static BlockCancelActionStoppingMovementPatch& Get()
    {
        static BlockCancelActionStoppingMovementPatch instance;
        return instance;
    }
};
