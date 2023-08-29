#include "SetRotateFlagOverwrite.hpp"
#include "../State.hpp"

bool SetRotateFlagOverwrite::Prepare()
{
    SetRotateFlagOverwriteAsm patch;
    patch.ready();

    auto match = Search();
    auto address = AsAddress(match);

    if (!address)
    {
        State::GetSingleton()->mod_found_all_addresses = false;
        WARN("SetRotateFlagOverwrite not found");
        return false;
    }

    INFO("Hooked SetRotateFlagOverwrite: {:X}", AsAddress(address));
    handle = dku::Hook::AddASMPatch(address, { 0, 6 }, &patch);
    return true;
}

void SetRotateFlagOverwrite::Enable() { handle->Enable(); }

void* SetRotateFlagOverwrite::Search(uintptr_t a_base)
{
    return dku::Hook::Assembly::search_pattern<
        "32 83 A9 00 00 00 88">(a_base);
}
