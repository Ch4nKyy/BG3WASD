#pragma once

#include "Hooks/Base/FunctionHook.hpp"

class CharacterDeathHook : public FunctionHook
{
public:
    CharacterDeathHook() :
        FunctionHook({ search_pattern<"E8 ?? ?? ?? ?? 90 4C ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? 48 ?? ?? "
                                      "E8 ?? ?? ?? ?? 4C ?? ?? 4C ?? "
                                      "?? ?? 48 ?? ?? ?? 48 ?? ?? E8 ?? ?? ?? ?? 48 ?? ?? 74 ?? BA "
                                      "?? ?? ?? ?? 48 ?? ?? E8">() },
            { 57 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static bool OverrideFunc(int64_t a1, int64_t a2);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static CharacterDeathHook& Get()
    {
        static CharacterDeathHook instance;
        return instance;
    }
};
