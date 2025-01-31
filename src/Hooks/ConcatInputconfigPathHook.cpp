#include <codecvt>
#include <locale>

#include "Settings.hpp"
#include "State.hpp"
#include "ConcatInputconfigPathHook.hpp"

void ConcatInputconfigPathHook::EnableSpecifically(uintptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

QWORD* ConcatInputconfigPathHook::OverrideFunc(QWORD* Src, uint32_t* a2, void* a3)
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    QWORD* return_value = OriginalFunc(Src, a2, a3);

    if (state->inputconfig_path == L"")
    {
        char* inputconfig_path_char = *(char**)return_value;
        std::string inputconfig_path_string{ inputconfig_path_char };
        if (dku::string::icontains(inputconfig_path_string, "3/PlayerProfiles/"))
        {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            std::wstring inputconfig_path_wstring = converter.from_bytes(inputconfig_path_string);
            state->inputconfig_path = inputconfig_path_wstring;
            INFO("Using inputconfig path: {}", inputconfig_path_string);
        }
    }

    return return_value;
}
