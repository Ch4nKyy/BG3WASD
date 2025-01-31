#pragma once

#include "Hooks/Base/FunctionHook.hpp"

class CombatEndHook : public FunctionHook
{
public:
    CombatEndHook() :
        FunctionHook({ search_pattern<"83 ?? ?? ?? 72 ?? 48 ?? ?? ?? E8 ?? ?? ?? ?? 90 49 ?? ?? 49 "
                                      "?? ?? ?? ?? ?? ?? E8 ?? ?? ?? "
                                      "?? 48 ?? ?? 74 ?? 48 ?? ?? ?? E8 ?? ?? ?? ?? 48 ?? ?? 48 ?? "
                                      "?? ?? E8 ?? ?? ?? ?? 90">() },
            { 40 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static const char* OverrideFunc(DWORD* a1);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static CombatEndHook& Get()
    {
        static CombatEndHook instance;
        return instance;
    }
};
