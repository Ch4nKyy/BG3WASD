#pragma once

#include "Hooks/Base/FunctionHook.hpp"

// Caller: Unknown. Probably pretty low level.
// Target: UpdateCamera
// Thread: GameThread
// Called when: Every frame, but not in dialog.
// Purpose: Detects when the camera is in combat mode, and toggles the character movement mode accordingly.
//
// Also good for storing the current camera pointer, and potentially other camera related stuff in the future.
class UpdateCameraHook : public FunctionHook
{
public:
    UpdateCameraHook() :
        FunctionHook({ search_pattern<"88 ?? ?? 4C ?? ?? ?? 4D ?? ?? 49 ?? ?? 49 ?? ?? E8">() },
            { 16 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static int64_t OverrideFunc(uint64_t a1, uint64_t a2, uint64_t a3, int64_t a4);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static UpdateCameraHook& Get()
    {
        static UpdateCameraHook instance;
        return instance;
    }
};
