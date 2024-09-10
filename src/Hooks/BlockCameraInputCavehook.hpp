#pragma once

using namespace DKUtil::Alias;

class BlockCameraInputCavehook
{
public:
    static void Enable();
    static bool Prepare();

private:
    static bool __cdecl Func();
    static inline std::array<uintptr_t, 1> addresses;
    static inline bool all_found = false;
    static inline std::unique_ptr<DKUtil::Hook::CaveHookHandle> handle;
};
