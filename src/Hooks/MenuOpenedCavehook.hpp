#pragma once

#include "Hooks/Base/CaveHook.hpp"

// Caller: MenuToggled
// Target: Cave
// Thread: GameThread
// Called when: A menu is opened (e.g. inventory, map, examine window, etc.)
//              When another menu was already open, this is not called.
// Purpose: Detect when a menu is opened, to update camera rotation.
//
// Does not trigger on the world context menu, which is handled by ContextMenuOpenedHook.
class MenuOpenedCavehook : public CaveHook
{
public:
    MenuOpenedCavehook() :
        CaveHook({ search_pattern<"45 ?? ?? 0F ?? ?? ?? ?? ?? 41 ?? ?? ?? ?? ?? 41 ?? ?? E9">() },
            { 9 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static void Func();

    static MenuOpenedCavehook& Get()
    {
        static MenuOpenedCavehook instance;
        return instance;
    }
};
