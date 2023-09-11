#include "WASDUnlock.hpp"
#include "../State.hpp"

void* WASDUnlock::Search(uintptr_t a_base = 0)
{
    return dku::Hook::Assembly::search_pattern<
        "38 ?? ?? ?? ?? ?? 0F ?? ?? ?? ?? ?? 48 ?? ?? ?? 8B ?? ?? ?? ?? ?? 39">(a_base);
}

bool WASDUnlock::Prepare()
{
    auto match1 = Search();
    address1 = AsAddress(match1);


    if (not(address1))
    {
        State::GetSingleton()->mod_found_all_addresses = false;
        WARN("WASD unlock not found!");
        return false;
    }

    return true;
}

void WASDUnlock::Enable()
{
    if (not(address1))
    {
        return;
    }

    static constexpr uint8_t data[0xC] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
        0x90, 0x90, 0x90 };

    dku::Hook::WriteImm(address1, data);
    DEBUG("WASD unlocked : {:X}", address1);

    return;
}
