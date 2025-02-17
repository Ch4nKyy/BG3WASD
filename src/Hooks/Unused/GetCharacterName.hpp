#pragma once

#include "Hooks/Base/FunctionHook.hpp"

class GetCharacterName : public FunctionHook
{
public:
    GetCharacterName() :
        FunctionHook({ search_pattern<"E8 ?? ?? ?? ?? 48 ?? ?? 48 ?? ?? ?? E8 ?? ?? ?? ?? 90 48 ?? "
                                      "?? ?? ?? ?? ?? 48 ?? ?? ?? E8 "
                                      "?? ?? ?? ?? 48 ?? ?? 48 ?? ?? ?? E8 ?? ?? ?? ?? 90 48 ?? ?? "
                                      "?? E8 ?? ?? ?? ?? 48 ?? ?? 48 "
                                      "?? ?? ?? E8 ?? ?? ?? ?? 90 48 ?? ?? ?? ?? ?? ?? 48">() },
            { 0 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static DWORD* OverrideFunc(DWORD* a1);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static GetCharacterName& Get()
    {
        static GetCharacterName instance;
        return instance;
    }
};
