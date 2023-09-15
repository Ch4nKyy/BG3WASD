#pragma once

typedef int64_t QWORD;

class CallSpecificCommandFunctionPre2Hook
{
public:
    static void Enable();
    static bool Prepare();
    static WORD* OverrideFunc(int64_t* a1, WORD* a2, int* command_struct);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

private:
    static inline std::array<uintptr_t, 1> addresses;
    static inline bool all_found = false;
};
