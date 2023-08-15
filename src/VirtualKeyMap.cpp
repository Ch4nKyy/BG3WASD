#include "VirtualKeyMap.hpp"

int VirtualKeyMap::GetByName(const std::string name)
{
    int vk = 0;
    if (VIRTUAL_KEY_MAP.contains(name))
    {
        vk = VIRTUAL_KEY_MAP.at(name);
    }
    return vk;
}
