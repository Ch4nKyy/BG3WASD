#pragma once

#include "Hooks/Base/FunctionHook.hpp"

typedef int64_t QWORD;

// Caller: LoadInputConfig
// Target: ConcatenatePath
// Thread: GameThread
// Called when: Inputconfig is loaded.
// Purpose: Sniff the inputconfig path.
//
// The path depends on the player profile.
class ConcatInputconfigPathHook : public FunctionHook
{
public:
    ConcatInputconfigPathHook() :
        FunctionHook({ search_pattern<"E8 ?? ?? ?? ?? 90 45 ?? ?? 45 ?? ?? 48 ?? ?? 49">() }, { 0 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static QWORD* OverrideFunc(QWORD* Src, uint32_t* a2, void* a3);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc = nullptr;

    static ConcatInputconfigPathHook& Get()
    {
        static ConcatInputconfigPathHook instance;
        return instance;
    }
};
