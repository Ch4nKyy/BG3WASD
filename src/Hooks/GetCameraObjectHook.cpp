#include "GetCameraObjectHook.hpp"
#include "../Addresses/IsInControllerMode.hpp"
#include "../Settings.hpp"
#include "../State.hpp"

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
        DEBUG("Hooked GetCameraObjectHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

int64_t GetCameraObjectHook::OverrideFunc(int64_t a1)
{
    auto* settings = Settings::GetSingleton();
    auto* state = State::GetSingleton();

    int64_t camera_object_ptr = OriginalFunc(a1);

    if (IsInControllerMode::Read())
    {
        return camera_object_ptr;
    }

    if (state->IsWasdCharacterMovement())
    {
        *(float*)(camera_object_ptr + 152) = 0.0f;  // x input
        *(float*)(camera_object_ptr + 156) = 0.0f;  // y input
        *(char*)(camera_object_ptr + 324) = 0;      // should move
    }

    // TODO character_leftright_is_rotate
    // if (*settings->character_leftright_is_rotate)
    // {
    //     *(char*)(camera_object_ptr + 497) = 0;      // is left/right rotating
    //     *(float*)(camera_object_ptr + 160) = 0.0f;  // left/right rotation
    // }

    bool new_combat_state = (*reinterpret_cast<bool*>(camera_object_ptr + 172) & 1) != 0;
    if (!state->combat_state_initiliazed || *settings->enable_auto_toggling_movement_mode &&
                                                new_combat_state != state->old_combat_state)
    {
        state->SetIsWasdCharacterMovement(!new_combat_state);
        state->old_combat_state = new_combat_state;
        state->combat_state_initiliazed = true;
    }

    return camera_object_ptr;
}
