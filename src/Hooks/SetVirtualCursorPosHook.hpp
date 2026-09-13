#pragma once

#include "Hooks/Base/FunctionHook.hpp"

typedef int64_t QWORD;

// Caller: PollSDLEvents
// Target: SetVirtualCursorPos
// Thread: MainThread
// Called when: a SDL mouse event is handled, once per frame.
// Purpose: When the cursor is overriden, e.g. invisible or centered, also override the virtual cursor position.
class SetVirtualCursorPosHook : public FunctionHook
{
public:
    SetVirtualCursorPosHook() :
        FunctionHook({ search_pattern<"F3 0F 11 4D ?? 4C ?? ?? ?? 48 ?? ?? ?? FF">() }, { 13 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static QWORD* OverrideFunc(int64_t* self, char* a2, int* a3);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static SetVirtualCursorPosHook& Get()
    {
        static SetVirtualCursorPosHook instance;
        return instance;
    }
};
