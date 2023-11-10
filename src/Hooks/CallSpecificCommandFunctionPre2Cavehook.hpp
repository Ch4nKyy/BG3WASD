#pragma once

typedef int64_t QWORD;

using namespace DKUtil::Alias;

// GameThread
// This hook is awesome.
// We can use it to detect input commands and react to them.
// But we can also use the original function (by using the self ptr) to send input commands
// directly instead of sending keys.
class CallSpecificCommandFunctionPre2Cavehook
{
public:
    static void Enable();
    static bool Prepare();
    static void Func(int64_t* self, WORD* a2, int* command_struct);

private:
    static inline std::array<uintptr_t, 1> addresses;
    static inline bool all_found = false;
    static inline std::unique_ptr<DKUtil::Hook::CaveHookHandle> handle;
};
