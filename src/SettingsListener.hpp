#pragma once
#include "DKUtil/Config.hpp"

#define CONFIG_PATH "NativeMods/BG3WASD.toml"sv

using namespace DKUtil::Alias;

class SettingsListener
{
public:
    static void Init();
    static void ListenForChanges();
    static unsigned int __stdcall ListenForChangesThread(void* param);

private:
    TomlConfig config = COMPILE_PROXY(CONFIG_PATH);

    inline static const std::string stop_message =
        "SettingsListener: Stopping. Config changes won't be detected.";
};
