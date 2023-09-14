#pragma once

class ASMPatchHandle;

struct BlockInteractMovePatchAsm : Xbyak::CodeGenerator
{
    BlockInteractMovePatchAsm()
    {
        nop();
        nop();
    }
};

class BlockInteractMovePatch
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
