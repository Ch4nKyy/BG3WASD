#include "QueueOpenContextMenuHook.hpp"
#include "Settings.hpp"
#include "State.hpp"

void QueueOpenContextMenuHook::EnableSpecifically(intptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

uint64_t QueueOpenContextMenuHook::OverrideFunc(int64_t system, int64_t screen_position,
    uint16_t* player_id, uint64_t* context_entity, void* target_entity)
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    return OriginalFunc(system, screen_position, player_id, context_entity, target_entity);
}
