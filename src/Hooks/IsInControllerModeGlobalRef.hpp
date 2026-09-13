#pragma once

#include "Hooks/Base/GlobalRef.hpp"

// Caller: Unknown
// Purpose: Detect if the game is currently in controller mode. If it is, we want to skip a lot of stuff,
//          because controller input should not be affected by the mod.
class IsInControllerModeGlobalRef : public GlobalRef<bool>
{
public:
    IsInControllerModeGlobalRef() :
        GlobalRef<bool>({ search_pattern<"80 3D ?? ?? ?? ?? ?? 74 22">() }, { 0 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static IsInControllerModeGlobalRef& Get()
    {
        static IsInControllerModeGlobalRef instance;
        return instance;
    }
};
