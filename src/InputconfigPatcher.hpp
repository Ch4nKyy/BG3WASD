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
    static std::vector<std::string> GetKeycombosOfCommandFromInputconfig(json data,
        std::string command, std::vector<std::string>& command_list, json default_keycombos);
    static bool IsStringEmptyOrWhitespace(const std::string string);
    static void ReadAndWriteInputconfig();
    static json UpdateData(json data, std::string move_command, std::string camera_command,
        json camera_default_keys);
    // If the user binds Camera Rotation to Rightclick, it will collide with the default bind
    // Alt+Rightclick of Ping. That is why we automatically remove this bind.
    static json FixPingCommand(json data);
    static void UpdateVkCombosOfCommandMap(json data, std::vector<std::string>& commands);
    static void ValidateKeys(json data);
    static void FindIssues(json data, const std::vector<std::string> commands,
        std::vector<std::string>& unbound_commands, std::vector<std::string>& not_found_keycombos,
        bool allow_modifiers);
    static void ValidateModHotkeys(std::vector<std::string>& mod_hotkeys_not_found_keycombos);
    static json GetKeycombosAsJsonObject(std::string keycombos_as_string);
};
