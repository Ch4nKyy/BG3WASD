#include "WalkSpeedHook.hpp"
#include "../Settings.hpp"
#include "../State.hpp"

void WalkSpeedHook::Enable()
{
    const std::uintptr_t caller_addresses[3] = {
        AsAddress(dku::Hook::Assembly::search_pattern<
            "E8 ?? ?? ?? ?? F3 0F ?? ?? F3 0F ?? ?? ?? F3 0F ?? ?? 80 ?? ?? ?? F3">()),
        AsAddress(dku::Hook::Assembly::search_pattern<
            "E8 ?? ?? ?? ?? 0F ?? ?? 48 ?? ?? F3 ?? ?? ?? ?? F3 ?? ?? ?? ?? ?? E8">()),
        AsAddress(dku::Hook::Assembly::search_pattern<
            "E8 ?? ?? ?? ?? F3 0F ?? ?? F3 0F ?? ?? ?? F3 0F ?? ?? F3 0F ?? ?? ?? 80">())
    };

    int i = 0;
    for (const auto& address : caller_addresses)
    {
        if (address)
        {
            _GetMovementInputVector =
                dku::Hook::write_call<5>(address, Hook_GetMovementInputVector);
            INFO("Hooked GetMovementInputVec #{}: {:X}", i, AsAddress(address))
        }
        else
        {
            FATAL("GetMovementInputVector #{} not found", i)
        }
        ++i;
    }
}

int64_t WalkSpeedHook::Hook_GetMovementInputVector(int64_t yx)
{
    yx = _GetMovementInputVector(yx);

    Vector3* yx_v = reinterpret_cast<Vector3*>(yx);

    auto* state = State::GetSingleton();
    if (state->walking ^ state->walking_or_sprint_held)
    {
        auto* settings = Settings::GetSingleton();
        yx_v->x *= *settings->walk_speed;
        yx_v->y *= *settings->walk_speed;
    }

    return yx;
}
