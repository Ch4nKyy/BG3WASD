#pragma once

class ASMPatchHandle;

struct BlockCancelActionStoppingMovementPatchAsm : Xbyak::CodeGenerator
{
    BlockCancelActionStoppingMovementPatchAsm() { test(rsp, rsp); }
};

// Blocks Cancel Action from stopping direct character movement.
// This is done so movement does not stutter, when rotating with rightclick AND
// Cancel does not block Context Menu, making using Context Menu out of movement easier.
class BlockCancelActionStoppingMovementPatch
{
public:
    static bool Prepare();
    static void Deactivate();
    static void Activate();
    static void Enable();

private:
    static inline std::array<uintptr_t, 1> addresses;
    static inline bool all_found = false;
    static inline std::unique_ptr<DKUtil::Hook::ASMPatchHandle,
        std::default_delete<DKUtil::Hook::ASMPatchHandle>>
        handle;
};
