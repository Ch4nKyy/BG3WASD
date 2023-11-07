#include "WASDUnlock.hpp"
#include "../State.hpp"

struct WASDUnlockPatch : Xbyak::CodeGenerator
{
    WASDUnlockPatch()
    {
        nop(); // 1
        nop();
        nop();
        nop();
        nop(); // 5
        nop();
        nop();
        nop();
        nop();
        nop(); // 10
        nop();
        nop();
        nop(); // 13
    }
};

void* WASDUnlock::Search(uintptr_t a_base = 0)
{
    return dku::Hook::Assembly::search_pattern<
        "44 ?? ?? ?? ?? ?? ?? 0F ?? ?? ?? ?? ?? 49 ?? ?? ?? 8B ?? ?? ?? ?? ?? 39 ?? ?? ?? ?? ?? "
        "0F">(a_base);
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

    WASDUnlockPatch patch;

    dku::Hook::WritePatch(address1, &patch);
    DEBUG("WASD unlocked : {:X}", address1);

    return;
}
