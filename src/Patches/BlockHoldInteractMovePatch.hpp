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
    }
};

class BlockHoldInteractMovePatch
{
public:
    static void SetDesiredState();
    static bool Prepare();

private:
    static void Enable();
    static void Disable();
    static inline std::array<uintptr_t, 1> addresses;
    static inline bool all_found = false;
    static inline std::unique_ptr<DKUtil::Hook::ASMPatchHandle,
        std::default_delete<DKUtil::Hook::ASMPatchHandle>>
        handle;
};
