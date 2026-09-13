#pragma once

#include "Hooks/Base/FunctionHook.hpp"

struct Command;

// Caller: ExecuteCharacterCommand
// Target: ExecuteCastAbilityCommand
// Thread: GameThread
// Called when: casting or cancelling an ability.
// Purpose: Allow rotating the camera while casting an ability, when CameraRotate and ActionCancel are bound to the
//          same key.
class ExecuteCastAbilityCommandHook : public FunctionHook
{
public:
    ExecuteCastAbilityCommandHook() :
        FunctionHook({ search_pattern<"E8 ?? ?? ?? 00 0F B7 5D F7">() }, { 0 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static WORD* OverrideFunc(WORD* a1, Command* command, uint64_t* a3, int64_t a4, int64_t a5,
        int64_t a6, int64_t a7, int64_t a8, int64_t a9, int64_t a10, int64_t a11, int64_t a12,
        int64_t a13, int64_t a14, int64_t a15, int64_t a16);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static ExecuteCastAbilityCommandHook& Get()
    {
        static ExecuteCastAbilityCommandHook instance;
        return instance;
    }
};
