#include "Hooks/Base/PatchHook.hpp"

void PatchHook::Activate()
{
    if (not found_all_addresses)
    {
        return;
    }

    for (auto& handle : handles)
    {
        handle->Enable();
    }
    is_active = true;
}

void PatchHook::Deactivate()
{
    if (not found_all_addresses)
    {
        return;
    }

    for (auto& handle : handles)
    {
        handle->Disable();
    }
    is_active = false;
}
