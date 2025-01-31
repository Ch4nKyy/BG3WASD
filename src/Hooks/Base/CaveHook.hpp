#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Hooks/Base/HookBase.hpp"

using namespace DKUtil::Alias;

/**
 * Base class for cave hooks.
 * Stores the handles.
 */
class CaveHook : public HookBase
{
public:
    CaveHook(std::vector<void*> search_patterns, std::vector<uintptr_t> offsets, std::string name,
        std::vector<void*> fallback_search_patterns = {}) :
        HookBase(search_patterns, offsets, name, fallback_search_patterns) {};

protected:
    static inline std::vector<std::unique_ptr<DKUtil::Hook::CaveHookHandle>> handles;
};
