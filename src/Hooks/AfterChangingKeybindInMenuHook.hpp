#pragma once

#include "Hooks/Base/FunctionHook.hpp"

// Caller: AfterChangingKeybindInMenu
// Target: AfterChangingKeybindInMenu2
// Thread: GameThread
// Called when: you leave the keybinding tab. (Before patch 7, it was called every time you changed a keybind.)
// Purpose: Patch the inputconfig, when it was changed ingame.
class AfterChangingKeybindInMenuHook : public FunctionHook
{
public:
    AfterChangingKeybindInMenuHook() :
        FunctionHook(
            { search_pattern<"48 83 BB ?? ?? 00 00 00 74 08 48 8B CB E8 ?? ?? EA FF">() },
            { 13 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static void OverrideFunc(int64_t a1);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc = nullptr;

    static AfterChangingKeybindInMenuHook& Get()
    {
        static AfterChangingKeybindInMenuHook instance;
        return instance;
    }
};
