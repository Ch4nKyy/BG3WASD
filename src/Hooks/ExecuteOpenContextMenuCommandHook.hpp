#pragma once

#include "Hooks/Base/FunctionHook.hpp"

struct Command;

// Caller: ExecuteCharacterCommand2
// Target: ExecuteOpenContextMenuCommand
// Thread: GameThread
// Called when: ContextMenu command is executed.
// Purpose: Prevent ActionCancel/ContextMenu from blocking camera rotation while interactmoving.
//
// Possibly, this is only necessary, when auto-rotate is enabled.
class ExecuteOpenContextMenuCommandHook : public FunctionHook
{
public:
    ExecuteOpenContextMenuCommandHook() :
        FunctionHook({ search_pattern<"E8 ?? ?? 04 00 0F B7 08">() }, { 0 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static int64_t OverrideFunc(int64_t a1, int64_t a2, Command* command, int16_t a4, int64_t a5);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static ExecuteOpenContextMenuCommandHook& Get()
    {
        static ExecuteOpenContextMenuCommandHook instance;
        return instance;
    }
};
