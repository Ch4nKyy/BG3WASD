#include "CombatEndHook.hpp"
#include "../State.hpp"

bool CombatEndHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "83 ?? ?? ?? 72 ?? 48 ?? ?? ?? E8 ?? ?? ?? ?? 90 49 ?? ?? 49 ?? ?? ?? ?? ?? ?? E8 ?? ?? ?? "
        "?? 48 ?? ?? 74 ?? 48 ?? ?? ?? E8 ?? ?? ?? ?? 48 ?? ?? 48 ?? ?? ?? E8 ?? ?? ?? ?? 90">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("CombatEndHook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void CombatEndHook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<5>(address + 40, OverrideFunc);
        DEBUG("Hooked CombatEndHook #{}: {:X}", i, AsAddress(address + 40));
        ++i;
    }
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
}
