#pragma once

#include "Hooks/Base/FunctionHook.hpp"

// Caller: FTBStart
// Target: LogTelemetry
// Thread: GameThread
// Called when: forced turn based mode starts.
// Purpose: Detect the start of FTB to auto toggle movement mode.
class FTBStartHook : public FunctionHook
{
public:
    FTBStartHook() :
        FunctionHook({ search_pattern<"4C 89 65 E0 4C 8D 05 ?? ?? ?? ?? 48 8D 55 D0 E8">() }, { 15 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static int64_t OverrideFunc(int64_t a1, int64_t a2, char* a3);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static FTBStartHook& Get()
    {
        static FTBStartHook instance;
        return instance;
    }
};
