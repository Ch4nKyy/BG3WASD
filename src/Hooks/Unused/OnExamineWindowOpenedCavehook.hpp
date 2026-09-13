#pragma once

#include "Hooks/Base/CaveHook.hpp"

// I wanted to use this to detect the state of the examine window, but now my is_menu_open includes that info.
class OnExamineWindowOpenedCavehook : public CaveHook
{
public:
    OnExamineWindowOpenedCavehook() :
        CaveHook({ search_pattern<"48 ?? ?? 48 ?? ?? ?? 48 ?? ?? 45 ?? ?? 4C ?? ?? ?? ?? 0F">() },
            { 0 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static void Func();

    static OnExamineWindowOpenedCavehook& Get()
    {
        static OnExamineWindowOpenedCavehook instance;
        return instance;
    }
};
