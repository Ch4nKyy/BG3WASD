#pragma once

#include "Hooks/Base/FunctionHook.hpp"

class SDL_GetWindowGrabHook : public FunctionHook
{
public:
    SDL_GetWindowGrabHook() :
        FunctionHook({ search_pattern<"FF ?? ?? ?? ?? ?? 3B ?? 74 ?? 48 ?? ?? ?? 8B ?? FF">() },
            { 0 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static int64_t OverrideFunc(int64_t a1);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static SDL_GetWindowGrabHook& Get()
    {
        static SDL_GetWindowGrabHook instance;
        return instance;
    }
};
