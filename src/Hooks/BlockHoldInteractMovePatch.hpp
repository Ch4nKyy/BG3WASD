#pragma once

#include "Hooks/Base/PatchHook.hpp"

struct BlockHoldInteractMovePatchAsm : Xbyak::CodeGenerator
{
    BlockHoldInteractMovePatchAsm()
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

// Caller: ExecuteMoveCommand
// Target: Interact/Action1 command section.
// Thread: GameThread
// Called when: holding interact to move.
// Purpose: Completely block interact hold moves.
//
// Patch should be activated, when the config is (re)loaded and BlockInteractMove is true.
class BlockHoldInteractMovePatch : public PatchHook
{
public:
    BlockHoldInteractMovePatch() :
        PatchHook({ search_pattern<"C7 46 20 00 01 00 00">() }, { 0 },
            std::source_location::current().function_name()) {};
    void EnableSpecifically(intptr_t address_incl_offset) override;

    static BlockHoldInteractMovePatch& Get()
    {
        static BlockHoldInteractMovePatch instance;
        return instance;
    }
};
