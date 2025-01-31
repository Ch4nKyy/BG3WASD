#include "CombatStartHook.hpp"
#include "State.hpp"

void CombatStartHook::EnableSpecifically(uintptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

const char* CombatStartHook::OverrideFunc(DWORD* a1)
{
    // const char* faction_name = OriginalFunc(a1);

    // char substring[10];
    // memcpy(substring, &faction_name[0], 9);
    // substring[9] = '\0';
    // if (strcmp(substring, "Companion") == 0 || strcmp(substring, "Hero Play") == 0)
    // {
    //     auto* state = State::GetSingleton();
    //     state->SetMovementModeToggled(false);
    // }

    // return faction_name;
    return nullptr;
}
