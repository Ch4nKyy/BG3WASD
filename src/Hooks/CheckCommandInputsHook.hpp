#pragma once

#include "Hooks/Base/FunctionHook.hpp"

class CheckCommandInputsHook : public FunctionHook
{
public:
    CheckCommandInputsHook() :
        FunctionHook({ search_pattern<"E8 ?? ?? ?? ?? 48 ?? ?? ?? 48 ?? ?? FF ?? ?? ?? ?? ?? 48 ?? "
                                      "?? ?? ?? ?? ?? 48 ?? ?? 48 ?? "
                                      "?? ?? ?? ?? ?? FF">() },
            { 0 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static char OverrideFunc(int64_t a1, float* a2);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static CheckCommandInputsHook& Get()
    {
        static CheckCommandInputsHook instance;
        return instance;
    }
};
