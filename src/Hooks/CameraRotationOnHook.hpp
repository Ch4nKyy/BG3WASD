#pragma once

#include "Hooks/Base/FunctionHook.hpp"

// Caller: Multiple, including ExecuteCameraCommand
// Target: CameraRotationOn
// Thread: GameThread
// Called when: Camera Rotate key was pressed.
// Purpose: Detect when camera rotation is turned on, to update camera rotation and cursor state.
class CameraRotationOnHook : public FunctionHook
{
public:
    CameraRotationOnHook() :
        FunctionHook(
            { search_pattern<"41 84 ?? 74 07 E8 ?? ?? ?? FF EB 05 E8 ?? ?? ?? FF 4C 8B">() }, { 5 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static void OverrideFunc(int64_t a1, int a2);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static CameraRotationOnHook& Get()
    {
        static CameraRotationOnHook instance;
        return instance;
    }
};
