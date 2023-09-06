#pragma once

#include <array>

class GetInputValueHook
{
public:
    static void Enable();
    static bool Prepare();

private:
    static int64_t OverrideFunc(int64_t a1, int64_t a2, int64_t a3, uint8_t a4);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;
    static inline std::array<uintptr_t, 4> addresses;
    static inline bool all_found = false;
};
