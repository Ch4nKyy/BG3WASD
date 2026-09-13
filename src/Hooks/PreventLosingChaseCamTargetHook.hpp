#pragma once

#include "Hooks/Base/FunctionHook.hpp"

// Caller: UpdateCameraPosition
// Target: IsPointInsideEntityBounds
// Thread: GameThread
// Called when: Probably every frame, but not in dialog.
// Purpose: In character movement mode, prevent the chase cam from losing focus on the character.
//
// There is one situation where the chase cam can lose focus on the character, and I don't know why.
// It involves pressing movement, leftclick and camera rotation toggle at the same time, I think.
// However, returning 0 from this hooked function prevents the symptom. (Not the cause though.)
class PreventLosingChaseCamTargetHook : public FunctionHook
{
public:
    PreventLosingChaseCamTargetHook() :
        FunctionHook({ search_pattern<"B8 3C 00 00 00 "
                                      "49 8B D6 "
                                      "48 03 D0 "
                                      "E8 ?? ?? ?? ?? "
                                      "84 C0 "
                                      "75 ?? "
                                      "B0 01">() },
            { 11 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static char OverrideFunc(int64_t entity, int64_t point);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static PreventLosingChaseCamTargetHook& Get()
    {
        static PreventLosingChaseCamTargetHook instance;
        return instance;
    }
};
