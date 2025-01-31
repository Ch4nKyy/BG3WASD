#pragma once

#include "Hooks/Base/FunctionHook.hpp"

class AfterInitialLoadInputConfigHook : public FunctionHook
{
public:
    AfterInitialLoadInputConfigHook() :
        FunctionHook(
            { search_pattern<"E8 ?? ?? ?? ?? ?? ?? ?? ?? 22 ?? 41 80 E4 01">() },
            { 0 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static int64_t OverrideFunc(int64_t* a1, uint16_t a2);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc = nullptr;

    static AfterInitialLoadInputConfigHook& Get()
    {
        static AfterInitialLoadInputConfigHook instance;
        return instance;
    }
};
