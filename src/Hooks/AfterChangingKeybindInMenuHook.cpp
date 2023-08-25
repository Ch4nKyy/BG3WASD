#include "AfterChangingKeybindInMenuHook.hpp"
#include "../State.hpp"
#include "Addresses/LoadInputConfig.hpp"
#include "InputconfigPatcher.hpp"

bool AfterChangingKeybindInMenuHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "E8 ?? ?? ?? ?? 40 ?? ?? 48 ?? ?? 48 ?? ?? 74 ?? 4C ?? ?? ?? 48 ?? ?? ?? 45 ?? ?? 41 ?? ?? "
        "48 ?? ?? ?? ?? ?? ?? 40 ?? ?? ?? EB ?? 49 ?? ?? 48 ?? ?? 74">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("AfterChangingKeybindInMenuHook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void AfterChangingKeybindInMenuHook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<5>(address, Hook);
        INFO("Hooked AfterChangingKeybindInMenuHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

bool AfterChangingKeybindInMenuHook::IsValid() { return OriginalFunc != nullptr; }

int64_t AfterChangingKeybindInMenuHook::Hook(int64_t a1)
{
    auto ret = CallOriginal(a1);

    InputconfigPatcher::Patch();
    LoadInputConfig::Call(0, 0);

    return ret;
}

int64_t AfterChangingKeybindInMenuHook::CallOriginal(int64_t a1) { return OriginalFunc(a1); }
