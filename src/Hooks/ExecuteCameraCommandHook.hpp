#pragma once

#include "Hooks/Base/FunctionHook.hpp"

class Command;

// Caller: PrepareCameraCommand
// Target: ExecuteCameraCommand
// Thread: GameThread
// Called when: camera input commands are allowed and detected.
//              Does not handle edge panning.
//              Not during dialog, not during UI (except map UI).
// Purpose: Manipulate camera input commands.
//
// - Block camera movement during character movement mode.
// - Block camera rotate presses while it is toggled on
// - Promote the current camera handler candidate to the actual camera handler.
class ExecuteCameraCommandHook : public FunctionHook
{
public:
    ExecuteCameraCommandHook() :
        FunctionHook({ search_pattern<"E8 ?? ?? ?? ?? 0F ?? ?? 66 ?? ?? 80 ?? ?? 75">() }, { 0 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static char* OverrideFunc(uint64_t executor, char* result, void* context, Command* command);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static ExecuteCameraCommandHook& Get()
    {
        static ExecuteCameraCommandHook instance;
        return instance;
    }
};
