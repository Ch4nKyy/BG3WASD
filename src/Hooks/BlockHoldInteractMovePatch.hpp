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

// Completely blocks interact hold moves.
// Activated, when the config is (re)loaded and BlockInteractMove is true.
class BlockHoldInteractMovePatch : public PatchHook
{
public:
    BlockHoldInteractMovePatch() :
        PatchHook({ search_pattern<"C7 46 20 00 01 00 00">() }, { 0 },
            std::source_location::current().function_name()) {};
    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static BlockHoldInteractMovePatch& Get()
    {
        static BlockHoldInteractMovePatch instance;
        return instance;
    }
};
