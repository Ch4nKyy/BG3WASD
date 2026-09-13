#pragma once

#include <string>
#include <vector>

#include "Hooks/Base/HookBase.hpp"

/**
 * Base class for function references.
 * Provides a type-safe interface to call functions.
 */
template <typename Ret, typename... Args>
class FunctionRef : public HookBase
{
public:
    using Fn = Ret(__fastcall*)(Args...);

    FunctionRef(std::vector<void*> search_patterns, std::vector<intptr_t> offsets, std::string name,
        std::vector<void*> fallback_search_patterns = {}) :
        HookBase(search_patterns, offsets, name, fallback_search_patterns) {};

    Ret operator()(Args... args) const { return function(std::forward<Args>(args)...); }

    [[nodiscard]] bool IsValid() const { return function != nullptr; }

protected:
    Fn function = nullptr;
};
