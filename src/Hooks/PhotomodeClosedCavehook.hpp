#pragma once

#include "Hooks/Base/CaveHook.hpp"

struct Command;

// Caller: TogglePhotomodeOffPre
// Target: Shortly before call to virtual RotationForceToggledPre3
// Thread: GameThread
// Called when: When photo mode is deactivated.
// Purpose: Detect photo mode deactivation.
class PhotomodeClosedCavehook : public CaveHook
{
public:
    PhotomodeClosedCavehook() :
        CaveHook({ search_pattern<"4C ?? ?? 49 ?? ?? ?? 48 ?? ?? 4C ?? ?? ?? 49 ?? ?? ?? FF">() },
            { 0 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;
    static void Func();

    static PhotomodeClosedCavehook& Get()
    {
        static PhotomodeClosedCavehook instance;
        return instance;
    }
};
