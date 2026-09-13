#pragma once

#include "Hooks/Base/FunctionHook.hpp"

// Caller: DecideMoveUpdaterPre
// Target: DecideMoveUpdater
// Thread: GameThread
// Called when: movement is allowed, every frame.
// Purpose: Deactivate BlockCancelActionStoppingMovementPatch.
class DecideMoveUpdaterHook : public FunctionHook
{
public:
    DecideMoveUpdaterHook() :
        FunctionHook({ search_pattern<
                         "49 8B D4 48 8B CD E8 ?? ?? ?? ?? 4C 8B 35 ?? ?? ?? ?? 48 8B 45 08">() },
            { 6 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static int64_t OverrideFunc(int64_t a1, int64_t a2);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static DecideMoveUpdaterHook& Get()
    {
        static DecideMoveUpdaterHook instance;
        return instance;
    }
};
