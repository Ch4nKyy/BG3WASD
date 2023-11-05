#pragma once

class ASMPatchHandle;

struct BlockAnalogStickSelection2PatchAsm : Xbyak::CodeGenerator
{
    BlockAnalogStickSelection2PatchAsm()
    {
        mov(eax, 2);
        nop();
        nop();
        nop();
        nop();
        nop();
        nop();
        nop();
    }
};

// On game start, set Analog Stick Selection to disabled (2).
class BlockAnalogStickSelection2Patch
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
