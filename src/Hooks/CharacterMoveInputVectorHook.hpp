#pragma once

#include <array>

class CharacterMoveInputVectorHook
{
public:
    static void Enable();
    static bool Prepare();

private:
    static int64_t OverrideFunc(int64_t yx);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;
    static inline std::array<uintptr_t, 3> addresses;
    static inline bool all_found = false;
};
