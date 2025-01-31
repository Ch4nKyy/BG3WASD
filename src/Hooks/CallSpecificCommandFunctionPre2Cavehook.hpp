#pragma once

#include "Hooks/Base/CaveHook.hpp"

// GameThread
// This hook is awesome.
// We can use it to detect input commands and react to them.
// But we can also use the original function (by using the self ptr) to send input commands
// directly instead of sending keys.
class CallSpecificCommandFunctionPre2Cavehook : public CaveHook
{
public:
    CallSpecificCommandFunctionPre2Cavehook() :
        CaveHook(
            { search_pattern<"FF 50 68 48 8B C3">() },
            { 0 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(uintptr_t address_incl_offset) override;
    static void Func(int64_t* self, WORD* a2, int* command_struct);

    static CallSpecificCommandFunctionPre2Cavehook& Get()
    {
        static CallSpecificCommandFunctionPre2Cavehook instance;
        return instance;
    }
};
