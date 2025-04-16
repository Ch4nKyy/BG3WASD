#pragma once

#include "Hooks/Base/FunctionHook.hpp"

class SetCursorRotateHook : public FunctionHook
{
public:
    SetCursorRotateHook() :
        FunctionHook(
            { search_pattern<"41 84 ?? 74 07 E8 ?? ?? ?? FF EB 05 E8 ?? ?? ?? FF 4C 8B">() }, { 5 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static void OverrideFunc(int64_t a1, int a2);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static SetCursorRotateHook& Get()
    {
        static SetCursorRotateHook instance;
        return instance;
    }
};
