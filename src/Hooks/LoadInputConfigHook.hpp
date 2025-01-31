#pragma once

#include "Hooks/Base/FunctionHook.hpp"

class LoadInputConfigHook : public FunctionHook
{
public:
    LoadInputConfigHook() :
        FunctionHook(
            { search_pattern<"48 ?? ?? ?? ?? 48 ?? ?? ?? ?? 48 ?? ?? ?? ?? 55 41 ?? 41 ?? 41 ?? 41 "
                             "?? 48 ?? ?? 48 ?? ?? "
                             "?? 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? 48 ?? ?? ?? 44 ?? ?? ?? 88">() },
            { 0 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(uintptr_t address_incl_offset) override;

    using FunctionSignature = int64_t (*)(int64_t a1, char a2);
    static inline FunctionSignature OriginalFunc;

    static LoadInputConfigHook& Get()
    {
        static LoadInputConfigHook instance;
        return instance;
    }

    static void CallOriginal(int64_t a1, char a2)
    {
        if (OriginalFunc == nullptr)
        {
            return;
        }
        OriginalFunc(a1, a2);
    }
};
