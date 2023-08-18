#include "LoadInputConfig.hpp"
#include "../State.hpp"

bool LoadInputConfig::Prepare()
{
    uintptr_t address = AsAddress(dku::Hook::Assembly::search_pattern<
        "48 ?? ?? ?? ?? 48 ?? ?? ?? ?? 48 ?? ?? ?? ?? 55 41 ?? 41 ?? 48 ?? ?? ?? ?? 48 81 ?? ?? "
        "?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 ?? ?? 48 ?? ?? ?? 44 ?? ?? ?? 4C">());

    if (!address)
    {
        WARN("LoadInputConfig not found. Hotreloading the inputconfig will be disabled!");
        return false;
    }

    Func = reinterpret_cast<tLoadInputConfig>(address);
    INFO("LoadInputConfig found: {:X}", AsAddress(Func))
    return true;
}

bool LoadInputConfig::IsValid() { return Func != nullptr; }

int64_t LoadInputConfig::Call(int64_t a1, char a2)
{
    return Func(a1, a2);
}
