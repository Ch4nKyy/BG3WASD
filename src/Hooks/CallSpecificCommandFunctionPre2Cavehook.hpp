#pragma once

typedef int64_t QWORD;

using namespace DKUtil::Alias;

class CallSpecificCommandFunctionPre2Cavehook
{
public:
    static void Enable();
    static bool Prepare();
    static void Func(int64_t* a1, WORD* a2, int* command_struct);

private:
    static inline std::array<uintptr_t, 1> addresses;
    static inline bool all_found = false;
    static inline std::unique_ptr<DKUtil::Hook::CaveHookHandle> handle;
};
