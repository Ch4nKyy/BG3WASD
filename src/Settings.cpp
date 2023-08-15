#include "Settings.hpp"

void Settings::Load() noexcept
{
    static std::once_flag config_init;
    std::call_once(config_init,
        [&]()
        {
            config.Bind(toggle_walk_or_sprint, "VK_INSERT");
            config.Bind(hold_walk_or_sprint, "");
            config.Bind(walk_speed, 0.4);
            config.Bind(walking_is_default, FALSE);
        });

    config.Load();

    INFO("Config loaded successfully."sv)
}
