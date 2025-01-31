#pragma once

#include "Hooks/Base/FunctionHook.hpp"

typedef int64_t QWORD;

class ConcatInputconfigPathHook : public FunctionHook
{
public:
    ConcatInputconfigPathHook() :
        FunctionHook({ search_pattern<"E8 ?? ?? ?? ?? 90 45 ?? ?? 45 ?? ?? 48 ?? ?? 49">() }, { 0 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static QWORD* OverrideFunc(QWORD* Src, uint32_t* a2, void* a3);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc = nullptr;

    static ConcatInputconfigPathHook& Get()
    {
        static ConcatInputconfigPathHook instance;
        return instance;
    }
};
