#pragma once

#include "../State.hpp"

// Class is unused, but may be needed in the future.
struct SetRotateFlagOverwriteAsm : Xbyak::CodeGenerator
{
    SetRotateFlagOverwriteAsm()
    {
        // push(rbx);
        // mov(rbx, AsAddress(&State::GetSingleton()->should_rotate));
        // mov(al, ptr [rbx]);
        // pop(rbx);
    }
};

class SetRotateFlagOverwrite
{
public:
    static bool Prepare();
    static void Enable();

private:
    static void* Search(uintptr_t a_base = 0);

    static inline std::unique_ptr<DKUtil::Hook::ASMPatchHandle> handle;
};
