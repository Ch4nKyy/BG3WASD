#pragma once

#include "Hooks/Base/FunctionHook.hpp"

// Caller: UpdateContextMenuState
// Target: Cave where v18 + 112 = 0
// Thread: GameThread
// Called when: The world context menu is closed.
// Purpose: Detect when the world context menu is closed, to update camera rotation.
class ContextMenuClosedHook : public FunctionHook
{
public:
    ContextMenuClosedHook() :
        FunctionHook({ search_pattern<"48 ?? ?? ?? ?? ?? ?? 40 ?? ?? ?? 74 ?? 40 ?? ?? ?? 4C">() },
            { 32 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static int64_t OverrideFunc(int64_t a1, int64_t a2, DWORD* a3);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static ContextMenuClosedHook& Get()
    {
        static ContextMenuClosedHook instance;
        return instance;
    }
};
