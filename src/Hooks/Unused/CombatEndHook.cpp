#include "CombatEndHook.hpp"
#include "State.hpp"

void CombatEndHook::EnableSpecifically(uintptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

const char* CombatEndHook::OverrideFunc(DWORD* a1)
{
    // const char* faction_name = OriginalFunc(a1);
    // char faction_name_sub[10];
    // memcpy(faction_name_sub, &faction_name[0], 9);
    // faction_name_sub[9] = '\0';

    // auto* state = State::GetSingleton();
    // bool character_name_contains_player =
    //     state->combat_end_character_name.find("Player") != std::string::npos;

    // if (!state->last_dying_character_is_player &&
    //     (strcmp(faction_name_sub, "Companion") == 0 ||
    //         strcmp(faction_name_sub, "Hero Play") == 0) &&
    //     character_name_contains_player)
    // {
    //     auto* state = State::GetSingleton();
    //     state->SetMovementModeToggled(true);
    // }

    // return faction_name;
    return nullptr;
}
