#pragma once

#include "Hooks/Base/CaveHook.hpp"

class BlockCameraInputCavehook : public CaveHook
{
public:
    BlockCameraInputCavehook() :
        CaveHook({ search_pattern<"44 88 ?? 44 01 00 00 48">() }, { 0 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(uintptr_t address_incl_offset) override;
    static bool __cdecl Func();

    static BlockCameraInputCavehook& Get()
    {
        static BlockCameraInputCavehook instance;
        return instance;
    }
};
