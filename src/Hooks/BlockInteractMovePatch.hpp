#pragma once

#include "Hooks/Base/PatchHook.hpp"

struct BlockInteractMovePatchAsm : Xbyak::CodeGenerator
{
    BlockInteractMovePatchAsm() { test(rsp, rsp); }
};

// Completely blocks interact moves.
// Activated, when the config is (re)loaded and BlockInteractMove is true.
class BlockInteractMovePatch : public PatchHook
{
public:
    BlockInteractMovePatch() :
        PatchHook(
            { search_pattern<"41 83 38 04 48 8B ?? 48 8B ?? ?? 48 8B">() },
            { 0 }, std::source_location::current().function_name()) {};
    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static BlockInteractMovePatch& Get()
    {
        static BlockInteractMovePatch instance;
        return instance;
    }
};
