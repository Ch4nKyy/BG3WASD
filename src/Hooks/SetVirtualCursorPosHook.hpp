#pragma once

#include "Hooks/Base/FunctionHook.hpp"

typedef int64_t QWORD;

class SetVirtualCursorPosHook : public FunctionHook
{
public:
    SetVirtualCursorPosHook() :
        FunctionHook({ search_pattern<"F3 0F 11 4D ?? FF ?? 08 01 00 00 ?? ?? ?? ?? 48">() }, { 5 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static QWORD* OverrideFunc(int64_t* self, char* a2, int* a3);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static SetVirtualCursorPosHook& Get()
    {
        static SetVirtualCursorPosHook instance;
        return instance;
    }
};
