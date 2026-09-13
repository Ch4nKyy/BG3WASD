#include "PreventClickingActiveCharacterHook.hpp"
#include "Settings.hpp"
#include "State.hpp"
#include "Structs/EntityHandle.hpp"

void PreventClickingActiveCharacterHook::EnableSpecifically(intptr_t address_incl_offset)
{
    OriginalFunc = dku::Hook::write_call<5>(address_incl_offset, OverrideFunc);
}

bool PreventClickingActiveCharacterHook::OverrideFunc(EntityHandle* handle, int64_t* a2, char a3)
{
    auto* state = State::GetSingleton();
    auto* settings = Settings::GetSingleton();

    if (handle->value == state->active_character && *settings->prevent_clicking_active_character &&
        state->IsCharacterMovementMode())
    {
        return true;
    }

    return OriginalFunc(handle, a2, a3);
}
