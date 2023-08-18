#include "VirtualKeyMap.hpp"
#include <algorithm>

int VirtualKeyMap::GetByName(const std::string name)
{
    std::string name_copy = name;
    std::transform(name_copy.begin(), name_copy.end(), name_copy.begin(), ::tolower);
    if (VIRTUAL_KEY_MAP.contains(name_copy))
    {
        return VIRTUAL_KEY_MAP.at(name_copy);
    }
    std::string key_without_prefix = name_copy.substr(name_copy.find_last_of(":") + 1);
    if (key_without_prefix.length() == 1)
    {
        return int(key_without_prefix[0]);
    }
    return 0;
}
