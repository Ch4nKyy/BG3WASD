#pragma once

#include "Hooks/Base/FunctionHook.hpp"

class FTBStartHook : public FunctionHook
{
public:
    FTBStartHook() :
        FunctionHook({ search_pattern<"E8 ?? ?? ?? ?? 4C ?? ?? 4C ?? ?? ?? 48 ?? ?? 48 ?? ?? "
                                            "E8 ?? ?? ?? ?? 49 ?? ?? ?? 83 ?? ?? ?? 72 ?? 48">() },
            { 0 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static int64_t OverrideFunc(int64_t a1, int64_t a2, char* a3);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static FTBStartHook& Get()
    {
        static FTBStartHook instance;
        return instance;
    }
};
