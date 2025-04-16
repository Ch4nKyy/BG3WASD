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
    static json FixRightclickBinds(json data);
    static void UpdateVkCombosOfCommandMap(json data, std::vector<std::string>& commands);
    static void ValidateKeys(json data);
    static void FindIssues(json data, const std::vector<std::string> commands,
        std::vector<std::string>& unbound_commands, std::vector<std::string>& not_found_keycombos,
        bool allow_modifiers);
    static void ValidateModHotkeys(std::vector<std::string>& mod_hotkeys_not_found_keycombos);
    static json GetKeycombosAsJsonObject(std::string keycombos_as_string);

    inline static const std::string INPUTCONFIG_ERROR =
        "Inputconfig could not be patched!\n"
        "This error often occurs randomly. Please try again to change your hotkeys.\n"
        "If the error persists, try renaming or deleting "
        "C:/Users/xxxxx/AppData/Local/Larian Studios/Baldur's Gate "
        "3/PlayerProfiles/Public/inputconfig_p1.json. You can find this folder by pressing "
        "Win+R and typing %localappdata%.";
};
