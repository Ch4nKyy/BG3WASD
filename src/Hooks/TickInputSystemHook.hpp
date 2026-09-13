#pragma once

#include "Hooks/Base/FunctionHook.hpp"

// Caller: GameUpdateThreadProc
// Target: TickInputSystem
// Thread: GameThread
// Called when: once every frame.
// Purpose: Great multi-purpose hook. Ideal for things that need to be updated every frame.
//
// It's the entry point for input in the GameThread.
// It consumes input that was queued in the MainThread.
// It updates key states and context.
//
// - UpdateBlockCancelActionStoppingMovementPatch
// - Process queued commands (see InputFaker::QueueCommand)
// - Update camera rotation
class TickInputSystemHook : public FunctionHook
{
public:
    TickInputSystemHook() :
        FunctionHook(
            { search_pattern<
                "E8 ?? ?? ?? ?? 48 8B 4E 20 48 8B 09 FF 15 ?? ?? ?? ?? 48 8B 0D ?? ?? ?? ??">() },
            { 0 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static char OverrideFunc(int64_t a1, float* a2);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static TickInputSystemHook& Get()
    {
        static TickInputSystemHook instance;
        return instance;
    }
};
