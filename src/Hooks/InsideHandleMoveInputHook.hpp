#pragma once

typedef int64_t QWORD;

class InsideHandleMoveInputHook
{
public:
    static void Enable();
    static bool Prepare();

private:
    static WORD* OverrideFunc(int64_t a1, WORD* a2, uint16_t* a3, int64_t command_struct);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;
    static inline std::array<uintptr_t, 1> addresses;
    static inline bool all_found = false;
};
