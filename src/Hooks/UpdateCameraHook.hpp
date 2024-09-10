#pragma once

class UpdateCameraHook
{
public:
    static void Enable();
    static bool Prepare();

private:
    static int64_t OverrideFunc(uint64_t a1, uint64_t a2, uint64_t a3, int64_t a4);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;
    static inline std::array<uintptr_t, 1> addresses;
    static inline bool all_found = false;
};
