#pragma once

#include <array>

class CharacterMoveInputVectorHook
{
public:
    static void Enable();
    static bool Prepare();

private:
    static float* OverrideFunc(float* xyz, int64_t a2);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;
    static inline std::array<uintptr_t, 3> addresses;
    static inline bool all_found = false;
};
