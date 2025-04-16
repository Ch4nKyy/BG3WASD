#pragma once

#include "Hooks/Base/FunctionHook.hpp"

// This class is currently unused. It is prep work for mouselook toggle.
class WindowGainFocusHook : public FunctionHook
{
public:
    WindowGainFocusHook() :
        FunctionHook({ search_pattern<"E9 ?? ?? ?? ?? 8B ?? ?? 33 ?? 44 ?? ?? ?? 41 ?? "
                                      "?? 48 ?? ?? 48 ?? ?? ?? 8B ?? 48 ?? ?? ?? "
                                      "48 ?? ?? 0F ?? ?? ?? ?? ?? 44">() },
            { 0 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static void OverrideFunc();
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static WindowGainFocusHook& Get()
    {
        static WindowGainFocusHook instance;
        return instance;
    }
};
