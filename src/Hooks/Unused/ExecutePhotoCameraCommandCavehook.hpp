#pragma once

#include "Hooks/Base/CaveHook.hpp"

struct Command;

// Caller: ExecutePhotoCameraCommand
// Target: GetPhotoModePlayerInputState, but as a cavehook, because then we can also get the command object.
// Thread: GameThread
// Called when: When photo mode is active and player can input.
// Purpose: I tried to use it to detect is_in_photomode, but somehow it was less reliable or useful than detecting
//          the start and end of the photo mode.
class ExecutePhotoCameraCommandCavehook : public CaveHook
{
public:
    ExecutePhotoCameraCommandCavehook() :
        CaveHook({ search_pattern<"8B ?? 48 ?? ?? F2 ?? ?? ?? ?? 81">() }, { 0 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;
    static void Func(int64_t* camera_manager, int64_t a2, Command* command);

    static ExecutePhotoCameraCommandCavehook& Get()
    {
        static ExecutePhotoCameraCommandCavehook instance;
        return instance;
    }
};
