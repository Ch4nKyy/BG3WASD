#include "CombatStartHook.hpp"
#include "../State.hpp"

bool CombatStartHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "72 ?? 48 ?? ?? ?? E8 ?? ?? ?? ?? 90 48 ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 ?? "
        "?? 74 ?? 48 ?? ?? ?? E8 ?? ?? ?? ?? 48 ?? ?? 48 ?? ?? ?? E8 ?? ?? ?? ?? 90 48 ?? ?? ?? ?? "
        "?? ?? 48 ?? ?? ?? E8 ?? ?? ?? ?? 48 ?? ?? 48 ?? ?? ?? E8 ?? ?? ?? ?? 90">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("CombatStartHook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void CombatStartHook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<5>(address + 37, OverrideFunc);
        INFO("Hooked CombatStartHook #{}: {:X}", i, AsAddress(address + 37));
        ++i;
    }
}

const char* CombatStartHook::OverrideFunc(DWORD* a1)
{
    const char* faction_name = OriginalFunc(a1);

    char substring[10];
    memcpy(substring, &faction_name[0], 9);
    substring[9] = '\0';
    if (strcmp(substring, "Companion") == 0 || strcmp(substring, "Hero Play") == 0)
    {
        auto* state = State::GetSingleton();
        state->SetIsWasdCharacterMovement(false);
    }

    return faction_name;
}
