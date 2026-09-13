#pragma once

#include "Hooks/Base/FunctionHook.hpp"

// Caller: OpenContextMenuForTarget
// Target: QueueOpenContextMenu
// Thread: GameThread
// Called when: a context menu is requested to be opened for a hovered target.
// Purpose: I wanted this to prevent opening contextmenus on camera occluders, but it's not necessary.
//          This function uses the cached hover target to determine what the context menu should be opened for.
class QueueOpenContextMenuHook : public FunctionHook
{
public:
    QueueOpenContextMenuHook() :
        FunctionHook({ search_pattern<"0F 14 C1 66 48 0F 7E C2 E8 ?? ?? ?? ?? E9">(),
                         search_pattern<"0F 14 C7 4C 8D 4D ?? 66 48 0F 7E C2 4C 8B C6 E8">() },
            { 8, 15 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static uint64_t OverrideFunc(int64_t system, int64_t screen_position, uint16_t* player_id,
        uint64_t* context_entity, void* target_entity);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static QueueOpenContextMenuHook& Get()
    {
        static QueueOpenContextMenuHook instance;
        return instance;
    }
};
