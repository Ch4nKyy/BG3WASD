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

    static void UpdateData(json& data, const std::string& move_command,
        const std::string& camera_command, const json& camera_default_keys);

    // When binding camera rotate to rightclick, it can collide with the ping/alt key for various
    // reasons. In this function we do some tricks to avoid the collision.
    // It also collides with modifier keys!
    static void FixRightclickBindings(json& data);

    // It is important so support key:unknown and INVALID:unknown in the inputconfig.
    // It's the game's way of representing unbound keys.
    // The problem is that sometimes it binds 3 invalid keys to a command and then it becomes impossible to bind a valid
    // key to that command.
    static void FixInvalidBindings(json& data);

    static void UpdateVkCombosOfCommandMap(json data, std::vector<std::string>& commands);

    static void ValidateKeys(json data);

    // Iterate all commands and return which commands have invalid keybindings.
    static void FindIssues(json data, const std::vector<std::string> commands,
        std::vector<std::string>& not_found_keycombos, bool allow_modifiers);

    static void ValidateModHotkeys(std::vector<std::string>& mod_hotkeys_not_found_keycombos);

    static json GetKeycombosAsJsonObject(std::string keycombos_as_string);

    // Simply dumping the json file randomly failed in the past sometimes.
    // This function first writes to a temp file and then tries to override the inputconfig.
    // If it fails, it retries a few times.
    // Now, it seems to succeed on the first or at least second try.
    static bool WriteJson(const std::filesystem::path& path, const json& data);

    inline static const std::string INPUTCONFIG_ERROR =
        "Inputconfig could not be patched!\n"
        "This error often occurs randomly. Please try again to change your hotkeys.\n"
        "If the error persists, try renaming or deleting "
        "C:/Users/xxxxx/AppData/Local/Larian Studios/Baldur's Gate "
        "3/PlayerProfiles/Public/inputconfig_p1.json. You can find this folder by pressing "
        "Win+R and typing %localappdata%.";
};
