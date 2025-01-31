#pragma once

#include <string>
#include <vector>

#include "Hooks/Base/HookBase.hpp"

/**
 * Base class for function hooks.
 */
class FunctionHook : public HookBase
{
public:
    FunctionHook(std::vector<void*> search_patterns, std::vector<uintptr_t> offsets,
        std::string name, std::vector<void*> fallback_search_patterns = {}) :
        HookBase(search_patterns, offsets, name, fallback_search_patterns) {};
};
