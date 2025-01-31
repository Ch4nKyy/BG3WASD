#pragma once

#include "Hooks/Base/FunctionHook.hpp"

class CastOrCancelAbilityHook : public FunctionHook
{
public:
    CastOrCancelAbilityHook() :
        FunctionHook({ search_pattern<"E8 ?? ?? ?? 00 0F B7 5D">() }, { 0 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static WORD* OverrideFunc(WORD* a1, int* command_struct, uint64_t* a3, int64_t a4, int64_t a5,
        int64_t a6, int64_t a7, int64_t a8, int64_t a9, int64_t a10, int64_t a11, int64_t a12,
        int64_t a13, int64_t a14, int64_t a15, int64_t a16);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static CastOrCancelAbilityHook& Get()
    {
        static CastOrCancelAbilityHook instance;
        return instance;
    }
};
