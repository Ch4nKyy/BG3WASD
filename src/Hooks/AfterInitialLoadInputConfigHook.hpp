#pragma once

class AfterInitialLoadInputConfigHook
{
public:
    static void Enable();
    static bool Prepare();
    static int64_t CallOriginal(int64_t* a1, uint16_t a2);
    static bool IsValid();

private:
    static int64_t Hook(int64_t* a1, uint16_t a2);
    static inline std::add_pointer_t<decltype(Hook)> OriginalFunc = nullptr;
    static inline std::array<uintptr_t, 1> addresses;
    static inline bool all_found = false;
};
