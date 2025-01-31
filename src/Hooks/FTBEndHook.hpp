#pragma once

#include "Hooks/Base/FunctionHook.hpp"

class FTBEndHook : public FunctionHook
{
public:
    FTBEndHook() :
        FunctionHook({ search_pattern<"E8 ?? ?? ?? ?? 4D ?? ?? 4C ?? ?? ?? 48 ?? ?? 48 ?? ?? E8 ?? "
                                      "?? ?? ?? 49 ?? ?? E8">() },
            { 0 }, std::source_location::current().function_name(),
            { search_pattern<"E8 ?? ?? ?? ?? 4D ?? ?? 4C ?? ?? ?? 48 ?? ?? 48 ?? ?? E8">() }) {};

    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static int64_t OverrideFunc(int64_t a1, int64_t a2, char* a3);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static FTBEndHook& Get()
    {
        static FTBEndHook instance;
        return instance;
    }
};
