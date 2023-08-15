#pragma once

class WASDUnlock
{
public:
    static void Enable();

private:
    static void* Search(uintptr_t a_base);
};
