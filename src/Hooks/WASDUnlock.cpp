#include "WASDUnlock.hpp"
#include "../State.hpp"

void* WASDUnlock::Search(uintptr_t a_base = 0)
{
    return dku::Hook::Assembly::search_pattern<
        "38 05 ?? ?? ?? ?? 0F 84 ?? ?? ?? ?? 48 8B">(a_base);
}

bool WASDUnlock::Prepare()
{
    auto match1 = Search();
    address1 = AsAddress(match1);

    auto match2 = Search(address1 + 1);
    address2 = AsAddress(match2);

    if (not(address1 && address2))
    {
        State::GetSingleton()->mod_found_all_addresses = false;
        WARN("WASD unlock not found!");
        return false;
    }

    return true;
}

void WASDUnlock::Enable()
{
    if (not(address1 && address2))
    {
        return;
    }

    static constexpr uint8_t data[0xC] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
        0x90, 0x90, 0x90 };

    dku::Hook::WriteImm(address1, data);
    dku::Hook::WriteImm(address2, data);
    INFO("WASD unlocked : {:X} and {:X}", address1, address2);

    return;
}
