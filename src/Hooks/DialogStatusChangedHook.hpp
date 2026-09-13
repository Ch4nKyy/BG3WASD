#pragma once

#include "Hooks/Base/FunctionHook.hpp"

// Caller: DialogToggled
// Target: First function called.
// Thread: GameThread
// Called when: Dialog is toggled.
// Purpose: Detect when dialog is entered or exited, to update camera rotation.
class DialogStatusChangedHook : public FunctionHook
{
public:
    DialogStatusChangedHook() :
        FunctionHook(
            { search_pattern<
                "E8 ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? 33 ?? 44 ?? ?? ?? ?? ?? ?? ?? 90">() },
            { 0 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    // Normally this function does not take a second parameter, but handily the dialog status flag is already
    // in the RDX register.
    static void OverrideFunc(int64_t a1, bool a2);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static DialogStatusChangedHook& Get()
    {
        static DialogStatusChangedHook instance;
        return instance;
    }
};
