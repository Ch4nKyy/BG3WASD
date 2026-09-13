#pragma once

#include "Hooks/Base/FunctionHook.hpp"

// Caller: PollSDLEvents
// Target: SDL_PollEvent
// Thread: MainThread
// Called when: once every frame.
// Purpose: Manipulate mouse cursor.
class PollEventHook : public FunctionHook
{
public:
    PollEventHook() :
        FunctionHook({ search_pattern<"48 8D 4D ?? FF 15 ?? ?? ?? 04 85 C0 0F">() }, { 4 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static int64_t OverrideFunc(int64_t a1);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    // Calculate the squared distance between two points.
    static int32_t DistanceSquaredBetweenPoints(POINT a, POINT b);

    static PollEventHook& Get()
    {
        static PollEventHook instance;
        return instance;
    }
};
