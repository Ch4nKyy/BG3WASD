#pragma once

#include "Settings.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class InputconfigPatcher
{
public:
    static void Patch();

private:
    static bool IsStringEmptyOrWhitespace(const std::string string);
    static void ReadAndWriteInputconfig();
    static json UpdateData(json data, String& key, String& fallback_key,
        std::string controller_key, std::string controller_camera_key, std::string move_command,
        std::string camera_command);
};
