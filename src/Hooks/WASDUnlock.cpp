#include "WASDUnlock.hpp"

void* WASDUnlock::Search(uintptr_t a_base = 0)
{
    return dku::Hook::Assembly::search_pattern<
        "38 05 ?? ?? ?? ?? 0F 84 ?? ?? ?? ?? 48 8B ?? ?? 8B 05 ?? ?? ?? ?? 39 81 ?? ?? ?? ?? "
        "0F 84 ?? ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 8B">(a_base);
}

bool WASDUnlock::Enable()
{
    static constexpr uint8_t data[0xC] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
        0x90, 0x90, 0x90 };

    auto match1 = Search();
    auto addr1 = AsAddress(match1);

    auto match2 = Search(addr1 + 1);
    auto addr2 = AsAddress(match2);

    if (not(addr1 && addr2))
    {
        FATAL("Could not unlock WASD! Mod is disabled!");
        return false;
    }

    dku::Hook::WriteImm(addr1, data);
    dku::Hook::WriteImm(addr2, data);
    INFO("WASD unlocked : {:X} and {:X}", addr1, addr2);

    return true;
}
