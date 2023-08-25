#pragma once

#include "Settings.hpp"
#include "VirtualKeyMap.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class InputconfigPatcher
{
public:
    static void Patch();

private:
    static std::vector<std::string> GetBoundKeysOfCommand(json data, std::string command);
    static bool IsStringEmptyOrWhitespace(const std::string string);
    static void ReadAndWriteInputconfig();
    static json UpdateData(json data, std::string move_command, std::string camera_command,
        json camera_default_keys);
};
