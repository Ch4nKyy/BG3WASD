#pragma once

class ASMPatchHandle;

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
class BlockHoldInteractMovePatch
{
public:
    static void Activate();
    static void Deactivate();
    static bool Prepare();
    static void Enable();

private:
    static inline std::array<uintptr_t, 1> addresses;
    static inline bool all_found = false;
    static inline std::unique_ptr<DKUtil::Hook::ASMPatchHandle,
        std::default_delete<DKUtil::Hook::ASMPatchHandle>>
        handle;
};
