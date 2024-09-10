#pragma once

class AfterChangingKeybindInMenuHook
{
public:
    static void Enable();
    static bool Prepare();
    static void CallOriginal(int64_t a1);
    static bool IsValid();

private:
    static void Hook(int64_t a1);
    static inline std::add_pointer_t<decltype(Hook)> OriginalFunc = nullptr;
    static inline std::array<uintptr_t, 1> addresses;
    static inline bool all_found = false;
};
