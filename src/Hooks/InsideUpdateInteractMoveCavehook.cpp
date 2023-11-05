#include "InsideUpdateInteractMoveCavehook.hpp"
#include "../Settings.hpp"
#include "../State.hpp"

bool InsideUpdateInteractMoveCavehook::Prepare()
{
    return false;
    
    std::array<uintptr_t, 1> address_array = { AsAddress(
        dku::Hook::Assembly::search_pattern<"48 ?? ?? ?? ?? ?? ?? 4C ?? ?? ?? ?? ?? ?? 4C ?? ?? ?? "
                                            "?? ?? ?? 41 ?? ?? ?? ?? ?? ?? 74 ?? 41">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("InsideUpdateInteractMoveCavehook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void InsideUpdateInteractMoveCavehook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        DKUtil::Hook::Patch epilog = { "", 0x0 };

        handle = DKUtil::Hook::AddCaveHook(address, { 0, 6 }, FUNC_INFO(Func), nullptr, &epilog,
            DKUtil::Hook::HookFlag::kRestoreAfterEpilog);
        handle->Enable();
        DEBUG("Hooked InsideUpdateInteractMoveCavehook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

void InsideUpdateInteractMoveCavehook::Func()
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    state->SetCurrentlyInteractMoving(true);
}
