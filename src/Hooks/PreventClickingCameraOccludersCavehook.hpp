#pragma once

#include "Hooks/Base/CaveHook.hpp"
#include "Structs/Vector3.hpp"

// Caller: UpdateHoverTargetFromRaycast
// Target: Raycast
// Thread: GameThread
// Called when: Probably when the hover target is updated, every frame?
// Purpose: In character movement mode, modify the raycast start pos to prevent missclicking your trailing companions.
//
// The start of the raycast is offset by the current zoom.
// So the raytrace doesn't start from the screen plane, but from a plane that originates at the character.
class PreventClickingCameraOccludersCavehook : public CaveHook
{
public:
    PreventClickingCameraOccludersCavehook() :
        CaveHook({ search_pattern<"48 8D 57 0C FF 50 40">() }, { 0 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static bool Func(int64_t* self, Vector3* start, Vector3* end, float* results, int32_t flags,
        int32_t mask1, int32_t mask2, int32_t option1, int32_t option2, int32_t option3,
        int64_t* options);

    static PreventClickingCameraOccludersCavehook& Get()
    {
        static PreventClickingCameraOccludersCavehook instance;
        return instance;
    }
};
