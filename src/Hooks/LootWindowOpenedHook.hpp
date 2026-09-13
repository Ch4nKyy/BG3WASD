#pragma once

#include "Hooks/Base/FunctionHook.hpp"

// Caller: OnLootWindowOpened
// Target: HashSetInsert
// Thread: GameThread
// Called when: a loot window is opened.
// Purpose: Detect when a loot window is opened, to update camera rotation.
//
// The target function returns (bool inserted, int32_t new_index).
//
// The AOB signature is terrible, but the code around the target is so generic that the signature would have to be
// super long otherwise.
class LootWindowOpenedHook : public FunctionHook
{
public:
    LootWindowOpenedHook() :
        FunctionHook({ search_pattern<"E8 42 92 41 FF">() }, { 0 },
            std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static int64_t OverrideFunc(int64_t hash_set, int64_t result, uint64_t* key);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc = nullptr;

    static LootWindowOpenedHook& Get()
    {
        static LootWindowOpenedHook instance;
        return instance;
    }
};
