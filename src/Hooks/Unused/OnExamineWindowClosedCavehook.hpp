#pragma once

#include "Hooks/Base/CaveHook.hpp"

// I wanted to use this to detect the state of the examine window, but now my is_menu_open includes that info.
class OnExamineWindowClosedCavehook : public CaveHook
{
public:
    OnExamineWindowClosedCavehook() :
        CaveHook({ search_pattern<
                     "48 ?? ?? 48 ?? ?? 74 ?? 48 ?? ?? ?? 48 ?? ?? ?? FF ?? 48 ?? ?? 4C">() },
            { 0 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static void Func();

    static OnExamineWindowClosedCavehook& Get()
    {
        static OnExamineWindowClosedCavehook instance;
        return instance;
    }
};
