#pragma once

#include <string>
#include <vector>

using dku::Hook::Assembly::search_pattern;

/**
 * Base class for all function hooks, cave hooks, patches, etc.
 * Handles finding addresses for patterns and enabling hooks.
 * Every derived class must implement EnableSpecifically(). Doing this bit with templates instead is
 * difficult with DKUtils C function style.
 * Every derived class should implement a singleton Get() function so it is easy to get.
 * TODO or store the instances in the featureset for more isolation? what if one hook is required
 * for multiple features though?
 */
class HookBase
{
public:
    /**
     * fallback_search_patterns are used, when a original search_pattern is not found.
     * Necessary, when GOG and Steam versions are too different.
     */
    HookBase(std::vector<void*> search_patterns, std::vector<uintptr_t> offsets, std::string name,
        std::vector<void*> fallback_search_patterns = {}) : offsets(offsets), name(name)
    {
        if (search_patterns.size() > 0)
        {
            found_all_addresses = true;
        }

        for (std::size_t i = 0; i < search_patterns.size(); ++i)
        {
            auto address = AsAddress(search_patterns[i]);
            if (!address && fallback_search_patterns.size() > 0)
            {
                address = AsAddress(fallback_search_patterns[i]);
            }
            if (!address)
            {
                // State::GetSingleton()->mod_found_all_addresses = false;
                WARN("{} #{} not found", name, i);
                found_all_addresses = false;
                continue;
            }

            addresses.push_back(address);
        }
    };

    virtual void Enable();

    // Specific hooks must implement this.
    virtual void EnableSpecifically(uintptr_t address_incl_offset) = 0;

    bool IsValid() { return found_all_addresses; }

    std::string GetName() { return name; }

protected:
    bool found_all_addresses = false;
    std::vector<uintptr_t> addresses;
    std::vector<uintptr_t> offsets;
    std::string name;
};
