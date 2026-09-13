#pragma once

#include "Hooks/Base/FunctionHook.hpp"

// Caller: Multiple, including ExecuteCameraCommand
// Target: CameraRotationOff
// Thread: GameThread
// Called when: Camera Rotate key is released or forced to release by game, e.g. dialog.
// Purpose: Detect when camera rotation is turned off, to update camera rotation and cursor state.
//
// This one is probably difficult to detect if it was sent artificially, because it is sometimes called multiple times
// and from different places on one release event.
//
// I'm wondering if I should split this into 2. One for the first case that is called in the ExecuteCameraCommand
// function, and one for the other two cases that are called by the game to force off rotation, e.g. on dialog start.
class CameraRotationOffHook : public FunctionHook
{
public:
    CameraRotationOffHook() :
        FunctionHook({ search_pattern<"41 84 ?? 74 07 E8 ?? ?? ?? FF EB 05 E8 ?? ?? ?? FF 4C 8B">(),
                         search_pattern<"66 89 B7 F0 01 00 00 48 85 C9 74 ?? 8D 56 2E E8">(),
                         search_pattern<"48 85 C9 74 0A BA 2E 00 00 00 E8 ?? ?? ?? FF ?? 8B">() },
            { 12, 15, 10 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static void OverrideFunc(int64_t a1, int a2);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static CameraRotationOffHook& Get()
    {
        static CameraRotationOffHook instance;
        return instance;
    }
};
