#pragma once

#include "Hooks/Base/FunctionHook.hpp"

// Caller: PollSDLEvents
// Target: SDL_GetWindowGrab
// Thread: MainThread
// Called when: the window is created or tabbed in.
// Purpose: Get a handle of the sdl window for later.
class SDL_GetWindowGrabHook : public FunctionHook
{
public:
    SDL_GetWindowGrabHook() :
        FunctionHook({ search_pattern<"FF ?? ?? ?? ?? ?? 3B ?? 74 ?? 8B ?? 48 ?? ?? ?? FF">() },
            { 0 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static int64_t OverrideFunc(int64_t a1);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static SDL_GetWindowGrabHook& Get()
    {
        static SDL_GetWindowGrabHook instance;
        return instance;
    }
};
