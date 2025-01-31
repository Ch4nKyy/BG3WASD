#pragma once

#include "Hooks/Base/FunctionHook.hpp"

// Deactivates BlockCancelActionStoppingMovementPatch
class DecideMoveUpdaterHook : public FunctionHook
{
public:
    DecideMoveUpdaterHook() :
        FunctionHook({ search_pattern<"E8 ?? ?? ?? ?? 4C ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? 48 ?? ?? ?? "
                                      "?? 48 ?? ?? ?? ?? 49">() },
            { 0 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static int64_t OverrideFunc(int64_t a1, int64_t a2);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static DecideMoveUpdaterHook& Get()
    {
        static DecideMoveUpdaterHook instance;
        return instance;
    }
};
