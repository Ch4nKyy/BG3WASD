#pragma once

class WASDUnlock
{
public:
    static bool Enable();

private:
    static void* Search(uintptr_t a_base);
};
