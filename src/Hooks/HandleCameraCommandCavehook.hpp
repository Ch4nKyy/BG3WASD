#pragma once

#include "Hooks/Base/CaveHook.hpp"

struct Command;

// Caller: HandleCameraCommand
// Target: inside
// Thread: GameThread
// Called when: a camera command is handled.
// Purpose: Store the handler.
//
// In this case, the handler is only a candidate, because HandleCameraCommand is called by multiple handlers.
// Whether the handler is really the actual handler is determined in ExecuteCameraCommand.
//
// To manipulate camera input, ExecuteCameraCommand is better suited.
class HandleCameraCommandCavehook : public CaveHook
{
public:
    HandleCameraCommandCavehook() :
        CaveHook({ search_pattern<"0F ?? ?? ?? ?? ?? ?? 3C ?? 74 ?? 41 ?? ?? ?? 75">() }, { 0 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;
    static void Func(uint64_t* handler, WORD* result, Command* command);

    static HandleCameraCommandCavehook& Get()
    {
        static HandleCameraCommandCavehook instance;
        return instance;
    }
};
