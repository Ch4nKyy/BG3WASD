#pragma once

class CombatEndHook
{
public:
    static void Enable();
    static bool Prepare();

private:
    static int64_t OverrideFunc(int64_t a1, int64_t a2, char* a3);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;
    static inline std::array<uintptr_t, 1> addresses;
    static inline bool all_found = false;
};
