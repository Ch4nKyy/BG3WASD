#pragma once

#include "Hooks/Base/FunctionHook.hpp"

class ResetCursorRotateHook : public FunctionHook
{
public:
    ResetCursorRotateHook() :
        FunctionHook({ search_pattern<"41 84 C5 74 07 E8 ?? ?? ?? FF EB 05 E8 ?? ?? ?? FF 4C 8B">(),
                         search_pattern<"41 ?? ?? ?? E8 ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? 44 ?? ?? "
                                        "?? ?? ?? ?? ?? ?? 0F">(),
                         search_pattern<"48 85 C9 74 0A BA 2E 00 00 00 E8 ?? ?? ?? FF ?? 8B">() },
            { 12, 4, 10 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static void OverrideFunc(int64_t a1, int a2);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static ResetCursorRotateHook& Get()
    {
        static ResetCursorRotateHook instance;
        return instance;
    }
};
