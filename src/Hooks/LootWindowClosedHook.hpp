#pragma once

#include "Hooks/Base/FunctionHook.hpp"

// Caller: OnLootWindowClosed
// Target: HashSetEraseAtIndex
// Thread: GameThread
// Called when: a loot window is closed.
// Purpose: Detect when all loot windows are closed, to update camera rotation.
//
// The target function returns the remaining number of loot windows.
class LootWindowClosedHook : public FunctionHook
{
public:
    LootWindowClosedHook() :
        FunctionHook({ search_pattern<"E8 ?? ?? ?? ?? 48 63 46 2C 3B D8 7D ?? 48 8B 4E 30 48 03 C0 "
                                      "0F 10 04 C1 48 8B C7 48 03 C0 0F 11 04 C1 48 8B 05">() },
            { 0 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static int64_t OverrideFunc(int64_t hash_set, int32_t index);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc = nullptr;

    static LootWindowClosedHook& Get()
    {
        static LootWindowClosedHook instance;
        return instance;
    }
};
