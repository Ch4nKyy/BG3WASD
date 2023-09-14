#pragma once

class ASMPatchHandle;

struct BlockCancelActionStoppingMovementPatchAsm : Xbyak::CodeGenerator
{
    BlockCancelActionStoppingMovementPatchAsm() { test(rsp, rsp); }
};

class BlockCancelActionStoppingMovementPatch
{
public:
    static bool Prepare();
    static void Disable();
    static void Enable();

private:
    static inline std::array<uintptr_t, 1> addresses;
    static inline bool all_found = false;
    static inline std::unique_ptr<DKUtil::Hook::ASMPatchHandle,
        std::default_delete<DKUtil::Hook::ASMPatchHandle>>
        handle;
};
