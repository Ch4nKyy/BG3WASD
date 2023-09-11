#include "GetCharacterName.hpp"
#include "../State.hpp"

bool GetCharacterName::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "E8 ?? ?? ?? ?? 48 ?? ?? 48 ?? ?? ?? E8 ?? ?? ?? ?? 90 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? E8 "
        "?? ?? ?? ?? 48 ?? ?? 48 ?? ?? ?? E8 ?? ?? ?? ?? 90 48 ?? ?? ?? E8 ?? ?? ?? ?? 48 ?? ?? 48 "
        "?? ?? ?? E8 ?? ?? ?? ?? 90 48 ?? ?? ?? ?? ?? ?? 48">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("GetCharacterName #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void GetCharacterName::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<5>(address, OverrideFunc);
        DEBUG("Hooked GetCharacterName #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

DWORD* GetCharacterName::OverrideFunc(DWORD* a1)
{
    DWORD* character_name_ptr = OriginalFunc(a1);

    const char* character_name_char_ptr = (const char*)character_name_ptr;
    int length = strlen(character_name_char_ptr);
    std::string character_name(character_name_char_ptr, length);
    auto* state = State::GetSingleton();
    state->combat_end_character_name = character_name;

    return character_name_ptr;
}
