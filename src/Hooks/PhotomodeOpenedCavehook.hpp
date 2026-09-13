#pragma once

#include "Hooks/Base/CaveHook.hpp"

struct Command;

// Caller: TogglePhotomodeOnPre
// Target: Shortly before call to virtual RotationForceToggledPre3
// Thread: GameThread
// Called when: When photo mode is activated.
// Purpose: Detect photo mode activation.
class PhotomodeOpenedCavehook : public CaveHook
{
public:
    PhotomodeOpenedCavehook() :
        CaveHook(
            { search_pattern<
                "48 ?? ?? 4C ?? ?? ?? 41 ?? ?? 48 ?? ?? 48 ?? ?? 41 ?? ?? EB ?? 48 ?? ?? 45">() },
            { 0 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;
    static void Func();

    static PhotomodeOpenedCavehook& Get()
    {
        static PhotomodeOpenedCavehook instance;
        return instance;
    }
};
