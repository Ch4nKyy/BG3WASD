#include "AfterChangingKeybindInMenuHook.hpp"
#include "State.hpp"
#include "InputconfigPatcher.hpp"

bool AfterChangingKeybindInMenuHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "48 83 BB ?? 07 00 00 00 74 08 48 8B CB E8 ?? ?? ?? FF 48 8B">()) };
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
        OriginalFunc = dku::Hook::write_call<5>(address + 13, Hook);
        DEBUG("Hooked AfterChangingKeybindInMenuHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

bool AfterChangingKeybindInMenuHook::IsValid() { return OriginalFunc != nullptr; }

// Before Patch 7, this was called everytime you changed a keybind, but now it is only called when 
// you go back from the keybind tab or when you switch tabs.
void AfterChangingKeybindInMenuHook::Hook(int64_t a1)
{
    CallOriginal(a1);

    InputconfigPatcher::Patch();
}

void AfterChangingKeybindInMenuHook::CallOriginal(int64_t a1) { OriginalFunc(a1); }
