#pragma once

#include "Hooks/Base/FunctionHook.hpp"

typedef int64_t QWORD;

// Caller: CreateSystemCursor
// Target: SDL_CreateSystemCursor
// Thread: GameThread
// Called when: game starts, once.
// Purpose: Create a crosshair cursor for the centered camera rotation cursor mode.
//
// The game usually calls the original function with a1 = 0, which creates an arrow cursor.
// The arrow cursor is probably never used in the game.
// We can use a1 = 3, which is the id for the systems crosshair cursor, and use it later!
class SDL_CreateSystemCursorHook : public FunctionHook
{
public:
    SDL_CreateSystemCursorHook() :
        FunctionHook({ search_pattern<"FF 15 ?? ?? ?? ?? 48 8B F0 48 8D 4B 28 48 8B D7">() }, { 0 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static int64_t OverrideFunc(QWORD a1);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static SDL_CreateSystemCursorHook& Get()
    {
        static SDL_CreateSystemCursorHook instance;
        return instance;
    }
};
