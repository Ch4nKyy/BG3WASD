#pragma once

#include <string>
#include <vector>

#include "Hooks/Base/HookBase.hpp"

class ASMPatchHandle;

/**
 * Base class for patches.
 * Stores the handles.
 * Does activation and deactivation.
 */
class PatchHook : public HookBase
{
public:
    PatchHook(std::vector<void*> search_patterns, std::vector<uintptr_t> offsets, std::string name,
        std::vector<void*> fallback_search_patterns = {}) :
        HookBase(search_patterns, offsets, name, fallback_search_patterns) {};

    void Activate();
    void Deactivate();

protected:
    std::vector<std::unique_ptr<DKUtil::Hook::ASMPatchHandle,
        std::default_delete<DKUtil::Hook::ASMPatchHandle>>>
        handles;
};
