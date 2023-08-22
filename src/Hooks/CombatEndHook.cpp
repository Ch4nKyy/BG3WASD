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
        INFO("Hooked CombatEndHook #{}: {:X}", i, AsAddress(address + 40));
        ++i;
    }
}

const char* CombatEndHook::OverrideFunc(DWORD* a1)
{
    const char* faction_name = OriginalFunc(a1);

    char substring[10];
    memcpy(substring, &faction_name[0], 9);
    substring[9] = '\0';
    auto* state = State::GetSingleton();
    if (!state->last_dying_character_is_player &&
        (strcmp(substring, "Companion") == 0 || strcmp(substring, "Hero Play") == 0))
    {
        auto* state = State::GetSingleton();
        state->is_wasd_character_movement = true;
    }

    return faction_name;
}
