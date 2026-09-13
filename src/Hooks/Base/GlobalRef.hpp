#pragma once

#include <string>
#include <vector>

#include "Hooks/Base/HookBase.hpp"

/**
 * Base class for global variable references.
 * Provides a type-safe interface to read and write global variables in memory.
 */
template <typename T>
class GlobalRef : public HookBase
{
public:
    GlobalRef(std::vector<void*> search_patterns, std::vector<intptr_t> offsets, std::string name,
        std::vector<void*> fallback_search_patterns = {}) :
        HookBase(search_patterns, offsets, name, fallback_search_patterns) {};

    T Read() const { return *value; }

    void Write(const T& new_value)
    {
        if (value)
        {
            *value = new_value;
        }
    }

    [[nodiscard]] bool IsValid() const { return value != nullptr; }

protected:
    T* value = nullptr;
};
