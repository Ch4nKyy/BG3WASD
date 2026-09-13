#pragma once

#include "Hooks/Base/CaveHook.hpp"

// Caller: MenuToggled
// Target: Cave
// Thread: GameThread
// Called when: The last open menu is closed. (e.g. inventory, map, examine window, etc.)
// Purpose: Detect when a menu is closed, to update camera rotation.
//
// Does not trigger on the world context menu, which is handled by ContextMenuOpenedHook.
class MenuClosedCavehook : public CaveHook
{
public:
    MenuClosedCavehook() :
        CaveHook({ search_pattern<"45 ?? ?? 7D ?? 41 ?? ?? ?? ?? ?? 45 ?? ?? 4C">() },
            { 5 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static void Func();

    static MenuClosedCavehook& Get()
    {
        static MenuClosedCavehook instance;
        return instance;
    }
};
