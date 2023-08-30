#include "GetCameraObjectHook.hpp"
#include "../Addresses/IsInControllerMode.hpp"
#include "../State.hpp"
#include "../Settings.hpp"

bool GetCameraObjectHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "E8 ?? ?? ?? ?? 48 ?? ?? 74 ?? F3 0F ?? ?? ?? 0F ?? ?? ?? ?? 48 ?? ?? ?? 5B C3">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("GetCameraObjectHook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void GetCameraObjectHook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<5>(address, OverrideFunc);
        INFO("Hooked GetCameraObjectHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

int64_t GetCameraObjectHook::OverrideFunc(int64_t a1)
{
    int64_t camera_object_ptr = OriginalFunc(a1);

    auto* state = State::GetSingleton();
    if (state->IsWasdCharacterMovement() && not IsInControllerMode::Read())
    {
        *(float*)(camera_object_ptr + 148) = 0.0f;  // x input
        *(float*)(camera_object_ptr + 152) = 0.0f;  // y input
        *(char*)(camera_object_ptr + 324) = 0;      // should move
    }

    bool new_combat_state = (*reinterpret_cast<bool*>(camera_object_ptr + 168) & 1 ) != 0;
    if (Settings::GetSingleton()->enable_auto_toggling_wasd_mode && new_combat_state!= state->old_combat_state)
    {
        state->SetIsWasdCharacterMovement(!new_combat_state);
        state->old_combat_state = new_combat_state;
    }

    return camera_object_ptr;
}
