#pragma once

class ASMPatchHandle;

struct BlockInteractMovePatchAsm : Xbyak::CodeGenerator
{
    BlockInteractMovePatchAsm()
    {
        test(rsp, rsp);
    }
};

// Completely blocks interact moves.
// Activated, when the config is (re)loaded and BlockInteractMove is true.
class BlockInteractMovePatch
{
public:
    static bool Prepare();
    static void Activate();
    static void Deactivate();
    static void Enable();

private:
    static inline std::array<uintptr_t, 1> addresses;
    static inline bool all_found = false;
    static inline std::unique_ptr<DKUtil::Hook::ASMPatchHandle,
        std::default_delete<DKUtil::Hook::ASMPatchHandle>>
        handle;
};
