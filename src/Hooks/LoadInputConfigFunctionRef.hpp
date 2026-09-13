#pragma once

#include "Hooks/Base/FunctionRef.hpp"

// Purpose: Let the game reload the inputconfig after we modify it.
class LoadInputConfigFunctionRef : public FunctionRef<void, int64_t /* a1 */, char /* a2 */>
{
public:
    LoadInputConfigFunctionRef() :
        FunctionRef({ search_pattern<"44 0F B6 FA 88 55 C0 48 8B F1 4C 8B 35 ?? ?? ?? ?? 48 8B 0D "
                                     "?? ?? ?? ?? 48 8B 01 FF 50 20">() },
            { -45 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static LoadInputConfigFunctionRef& Get()
    {
        static LoadInputConfigFunctionRef instance;
        return instance;
    }
};
