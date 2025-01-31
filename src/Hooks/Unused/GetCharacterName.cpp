#include "GetCharacterName.hpp"
#include "State.hpp"

void GetCharacterName::EnableSpecifically(uintptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
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
