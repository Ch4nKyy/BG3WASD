#pragma once

typedef int64_t QWORD;

class SetVirtualCursorPosHook
{
public:
    static void Enable();
    static bool Prepare();
    static void HideCursor(QWORD* xy);

private:
    static void OverrideFunc(QWORD* a1, QWORD* a2);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;
    static inline std::array<uintptr_t, 1> addresses;
    static inline bool all_found = false;
};
