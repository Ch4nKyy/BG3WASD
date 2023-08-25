#pragma once

class WASDUnlock
{
public:
    static void Enable();
    static bool Prepare();

private:
    static void* Search(uintptr_t a_base);
    static inline uintptr_t address1;
    static inline uintptr_t address2;
};
