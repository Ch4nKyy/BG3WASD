#pragma once

class CastOrCancelAbilityHook
{
public:
    static void Enable();
    static bool Prepare();

private:
    static WORD* OverrideFunc(WORD* a1, int* CommandStruct, uint64_t* a3, int64_t a4, int64_t a5,
        int64_t a6, int64_t a7, int64_t a8, int64_t a9, int64_t a10, int64_t a11, int64_t a12,
        int64_t a13, int64_t a14, int64_t a15, int64_t a16);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;
    static inline std::array<uintptr_t, 1> addresses;
    static inline bool all_found = false;
};
