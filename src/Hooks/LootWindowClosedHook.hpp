#pragma once

#include "Hooks/Base/FunctionHook.hpp"

// Caller: OnLootWindowClosed
// Target: HashSetEraseAtIndex
// Thread: GameThread
// Called when: a loot window is closed.
// Purpose: Detect when all loot windows are closed, to update camera rotation.
//
// The target function returns the remaining number of loot windows.
//
// The AOB signature is terrible, but the code around the target is so generic that the signature would have to be
// super long otherwise.
class LootWindowClosedHook : public FunctionHook
{
public:
    LootWindowClosedHook() :
        FunctionHook({ search_pattern<"E8 F0 AF 47 FF">() }, { 0 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static int64_t OverrideFunc(int64_t hash_set, int32_t index);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc = nullptr;

    static LootWindowClosedHook& Get()
    {
        static LootWindowClosedHook instance;
        return instance;
    }
};
