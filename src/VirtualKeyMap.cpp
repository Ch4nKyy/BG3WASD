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
        // Turn a-z to upper case, because the KeyboardProc codes are all upper case.
        // I used toupper() before, but it completely bugged other characters, so I do it manually.
        int code = int(key_without_prefix[0]);
        if (code >= 97 && code <= 122)
        {
            code -= 32;
        }
        return code;
    }
    return 0;
}
