#pragma once

typedef int64_t QWORD;

class SetVirtualCursorPosHook
{
public:
    static void Enable();
    static bool Prepare();

private:
    static QWORD* OverrideFunc(int64_t* self, char* a2, int* a3);
    static inline std::array<uintptr_t, 1> addresses;
    static inline bool all_found = false;
};
