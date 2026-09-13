#pragma once

#include "Hooks/Base/FunctionHook.hpp"

// Caller: OnLootWindowOpened
// Target: HashSetInsert
// Thread: GameThread
// Called when: a loot window is opened.
// Purpose: Detect when a loot window is opened, to update camera rotation.
//
// The target function returns (bool inserted, int32_t new_index).
class LootWindowOpenedHook : public FunctionHook
{
public:
    LootWindowOpenedHook() :
        FunctionHook({ search_pattern<"F3 0F 59 F6 49 63 5E 2C 4C 8D 45 ?? 48 8D 55 ?? 49 8B CE E8 "
                                      "?? ?? ?? ?? 49 63 46 28 41 3B 46 38">() },
            { 19 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static int64_t OverrideFunc(int64_t hash_set, int64_t result, uint64_t* key);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc = nullptr;

    static LootWindowOpenedHook& Get()
    {
        static LootWindowOpenedHook instance;
        return instance;
    }
};
