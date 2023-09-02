#include "CheckContextMenuOrCancelActionHook.hpp"
#include "../Settings.hpp"
#include "../State.hpp"

bool CheckContextMenuOrCancelActionHook::Prepare()
{
    std::array<uintptr_t, 1> address_array = { AsAddress(dku::Hook::Assembly::search_pattern<
        "E8 ?? ?? ?? ?? 0F ?? ?? 48 ?? ?? 66 ?? ?? 48 ?? ?? ?? 5B C3 BA">()) };
    addresses = address_array;

    all_found = true;
    int i = 0;
    for (const auto& address : addresses)
    {
        if (!address)
        {
            State::GetSingleton()->mod_found_all_addresses = false;
            WARN("CheckContextMenuOrCancelActionHook #{} not found", i);
            all_found = false;
        }
        ++i;
    }
    return all_found;
}

void CheckContextMenuOrCancelActionHook::Enable()
{
    if (not all_found)
    {
        return;
    }
    int i = 0;
    for (const auto& address : addresses)
    {
        OriginalFunc = dku::Hook::write_call<5>(address, OverrideFunc);
        INFO("Hooked CheckContextMenuOrCancelActionHook #{}: {:X}", i, AsAddress(address));
        ++i;
    }
}

int64_t CheckContextMenuOrCancelActionHook::OverrideFunc(int64_t a1, int64_t a2,
    int* SomeInputStruct, int16_t a4, int64_t a5)
{
    if (!*Settings::GetSingleton()->enable_improved_mouselook ||
        !*Settings::GetSingleton()->enable_rightclick_mouselook_fix)
    {
        return OriginalFunc(a1, a2, SomeInputStruct, a4, a5);
    }

    auto* state = State::GetSingleton();
    int command_id = *SomeInputStruct;
    if (command_id != 160 && command_id != 200)
    {
        return OriginalFunc(a1, a2, SomeInputStruct, a4, a5);
    }

    if (command_id == 160)  // cancel action
    {
        bool is_key_down = *(reinterpret_cast<bool*>(SomeInputStruct) + 28);
        if (is_key_down)
        {
            *(int*)(SomeInputStruct) = 0;
            state->last_time_cancel_action_pressed = SDL_GetTicks();
        }
        else
        {
            uint32_t time_now = SDL_GetTicks();
            uint32_t time_diff_millis = time_now - state->last_time_cancel_action_pressed;
            if (time_diff_millis > *Settings::GetSingleton()->rightclick_threshold)
            {
                *(int*)(SomeInputStruct) = 0;
            }
        }
    }
    if (command_id == 200)  // context menu
    {
        bool is_key_down = *(reinterpret_cast<bool*>(SomeInputStruct) + 28);
        if (is_key_down)
        {
            *(int*)(SomeInputStruct) = 0;
            state->last_time_context_menu_pressed = SDL_GetTicks();
        }
        else
        {
            uint32_t time_now = SDL_GetTicks();
            uint32_t time_diff_millis = time_now - state->last_time_context_menu_pressed;
            if (time_diff_millis > *Settings::GetSingleton()->rightclick_threshold)
            {
                *(int*)(SomeInputStruct) = 0;
            }
        }
    }
    return OriginalFunc(a1, a2, SomeInputStruct, a4, a5);
}
