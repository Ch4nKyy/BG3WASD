#pragma once

#include "Hooks/Base/FunctionHook.hpp"

// Caller: UpdateCameraPosition
// Target: GetViewport
// Thread: GameThread
// Called when: Probably every frame, but not in dialog.
// Purpose: In character movement mode, always block edge panning, because it causes the chase camera to lose focus on
//          the character.
//
// The target function normally gets the viewport data and checks if it is valid.
// We check in our hook if we want to block edge panning, and if so, we just return 0.
class BlockEdgePanningHook : public FunctionHook
{
public:
    BlockEdgePanningHook() :
        FunctionHook({ search_pattern<"E8 ?? ?? ?? ?? 48 8B F0 48 85 C0 0F 84 ?? ?? ?? ?? 44 38 2D">() },
            { 0 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static int64_t OverrideFunc(int64_t a1, int16_t a2);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static BlockEdgePanningHook& Get()
    {
        static BlockEdgePanningHook instance;
        return instance;
    }
};
