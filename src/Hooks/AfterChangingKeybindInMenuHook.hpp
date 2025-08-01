#pragma once

#include "Hooks/Base/FunctionHook.hpp"

class AfterChangingKeybindInMenuHook : public FunctionHook
{
public:
    AfterChangingKeybindInMenuHook() :
        FunctionHook(
            { search_pattern<"48 83 BB ?? ?? 00 00 00 74 08 48 8B CB E8 ?? ?? EA FF">() },
            { 13 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static void OverrideFunc(int64_t a1);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc = nullptr;

    static AfterChangingKeybindInMenuHook& Get()
    {
        static AfterChangingKeybindInMenuHook instance;
        return instance;
    }
};
