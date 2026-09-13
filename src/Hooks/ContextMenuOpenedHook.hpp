#pragma once

#include "Hooks/Base/FunctionHook.hpp"

// Caller: UpdateContextMenuState
// Target: Cave where v18 + 112 = 1
// Thread: GameThread
// Called when: The world context menu is opened.
// Purpose: Detect when the world context menu is opened, to update camera rotation.
class ContextMenuOpenedHook : public FunctionHook
{
public:
    ContextMenuOpenedHook() :
        FunctionHook(
            { search_pattern<
                "C6 ?? ?? ?? 4C ?? ?? ?? 48 ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? E9">() },
            { 19 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static int64_t OverrideFunc(int64_t a1, int64_t a2, DWORD* a3);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static ContextMenuOpenedHook& Get()
    {
        static ContextMenuOpenedHook instance;
        return instance;
    }
};
