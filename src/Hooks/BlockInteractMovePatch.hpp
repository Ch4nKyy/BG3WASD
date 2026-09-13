#pragma once

#include "Hooks/Base/PatchHook.hpp"

struct BlockInteractMovePatchAsm : Xbyak::CodeGenerator
{
    BlockInteractMovePatchAsm() { test(rsp, rsp); }
};

// Caller: ExecuteInteractMove
// Target: Section that checks command ID and pressed state.
// Thread: GameThread
// Called when: any sort of character commands are processed.
// Purpose: Block interact moves.
//
// Patch should be activated, when the config is (re)loaded and BlockInteractMove is true.
class BlockInteractMovePatch : public PatchHook
{
public:
    BlockInteractMovePatch() :
        PatchHook({ search_pattern<"41 83 38 04 48 8B ?? 48 8B ?? ?? 48 8B">() }, { 0 },
            std::source_location::current().function_name()) {};
    void EnableSpecifically(intptr_t address_incl_offset) override;

    static BlockInteractMovePatch& Get()
    {
        static BlockInteractMovePatch instance;
        return instance;
    }
};
