#pragma once

#include "Hooks/Base/FunctionHook.hpp"

/**
 * It's not a function hook, but an address to a variable, but it works very similarly.
 * Instead of setting an OriginalFunc and an OverrideFunc, we just get the address of the variable
 * and provide a Read function.
 */
class IsInControllerModeHook : public FunctionHook
{
public:
    IsInControllerModeHook() :
        FunctionHook({ search_pattern<"80 3D ?? ?? ?? ?? ?? 74 22">() }, { 0 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static void OverrideFunc(int64_t a1);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc = nullptr;

    static IsInControllerModeHook& Get()
    {
        static IsInControllerModeHook instance;
        return instance;
    }

    bool Read();

private:
    static inline bool* is_in_controller_mode = nullptr;
};
