#include "IsInControllerMode.hpp"
#include "../State.hpp"

bool IsInControllerMode::Prepare()
{
    uintptr_t address =
        AsAddress(dku::Hook::Assembly::search_pattern<"80 3D ?? ?? ?? ?? ?? 74 22">());

    if (!address)
    {
        State::GetSingleton()->mod_found_all_addresses = false;
        WARN("IsInControllerMode not found.");
        return false;
    }

    int32_t offset = *reinterpret_cast<int32_t*>(address + 2);
    is_in_controller_mode = reinterpret_cast<bool*>(address + 7 + offset);
    INFO("IsInControllerMode found {:X}", AsAddress(is_in_controller_mode))
    return true;
}

bool IsInControllerMode::Read()
{
    if (is_in_controller_mode == nullptr)
    {
        return false;
    }

    return *is_in_controller_mode;
}
