#pragma once

#include "Hooks/Base/FunctionHook.hpp"

struct EntityHandle;

// Caller: UpdateHoverTargetFromRaycast
// Target: IsObjectFadedOut
// Thread: GameThread
// Called when: ?
// Purpose: In character movement mode, make active character unclickable when the setting is enabled.
//
// IsObjectFadedOut usually just filters objects that are faded out (I think), but we extend it to also filter the
// active character when the setting is enabled, so you can click through the active character.
class PreventClickingActiveCharacterHook : public FunctionHook
{
public:
    PreventClickingActiveCharacterHook() :
        FunctionHook(
            { search_pattern<
                "E8 ?? ?? ?? ?? 84 C0 0F 85 ?? ?? ?? ?? 49 8B 0C 24 E8 ?? ?? ?? ?? 84 C0">() },
            { 0 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static bool OverrideFunc(EntityHandle* handle, int64_t* a2, char a3);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static PreventClickingActiveCharacterHook& Get()
    {
        static PreventClickingActiveCharacterHook instance;
        return instance;
    }
};
