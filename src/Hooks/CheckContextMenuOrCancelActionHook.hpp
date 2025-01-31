#pragma once

#include "Hooks/Base/FunctionHook.hpp"

class CheckContextMenuOrCancelActionHook : public FunctionHook
{
public:
    CheckContextMenuOrCancelActionHook() :
        FunctionHook({ search_pattern<"E8 ?? ?? 04 00 0F B7 08">() }, { 0 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static int64_t OverrideFunc(int64_t a1, int64_t a2, int* SomeInputStruct, int16_t a4,
        int64_t a5);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static CheckContextMenuOrCancelActionHook& Get()
    {
        static CheckContextMenuOrCancelActionHook instance;
        return instance;
    }
};
