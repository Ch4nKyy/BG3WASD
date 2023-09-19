#include <codecvt>
#include <locale>

#include "../Settings.hpp"
#include "../State.hpp"
#include "LoadStringHook.hpp"

bool LoadStringHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "E8 ?? ?? ?? ?? B2 ?? 48 ?? ?? ?? E8 ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? C7 ?? ?? "
        "?? ?? ?? ?? 48 ?? ?? ?? 48 ?? ?? ?? E8 ?? ?? ?? ?? 48 ?? ?? ?? E8 ?? ?? ?? ?? 90 C6 ?? ?? "
        "?? ?? 45 ?? ?? 45">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("LoadStringHook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void LoadStringHook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<5>(address, OverrideFunc);
        DEBUG("Hooked LoadStringHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

DWORD* LoadStringHook::OverrideFunc(DWORD* a1, int64_t a2)
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    if (state->profile_path == L"")
    {
        char* profile_path_char = *(char**)a2;
        std::string profile_path_string{ profile_path_char };
        if (dku::string::icontains(profile_path_string, "3/PlayerProfiles/"))
        {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            std::wstring profile_path_wstring = converter.from_bytes(profile_path_string);
            state->profile_path = profile_path_wstring;
            INFO("Using profile path: {}", profile_path_string);
        }
    }

    return OriginalFunc(a1, a2);
}
