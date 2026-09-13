#include "SettingsListener.hpp"
#include "Settings.hpp"

void SettingsListener::Init()
{
    _beginthreadex(NULL, 0, SettingsListener::ListenForChangesThread, NULL, 0, NULL);
}

unsigned int __stdcall SettingsListener::ListenForChangesThread(void* param)
{
    SettingsListener::ListenForChanges();
    return 0;
}

void SettingsListener::ListenForChanges()
{
    using namespace std::chrono;

    constexpr auto debounce_delay = 1000ms;

    const auto path = std::filesystem::current_path() / CONFIG_PATH;
    const auto cfg_filename = path.filename().wstring();
    const auto cfg_folder = path.parent_path().wstring();

    HANDLE file = CreateFileW(cfg_folder.c_str(), FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, nullptr);

    if (file == INVALID_HANDLE_VALUE)
    {
        WARN("SettingsListener: CreateFileW \"{}\" failed with error code {}",
            path.parent_path().string(), GetLastError());
        WARN(stop_message);
        return;
    }

    OVERLAPPED overlapped{};
    overlapped.hEvent = CreateEventW(nullptr, FALSE, FALSE, nullptr);

    if (!overlapped.hEvent)
    {
        WARN("SettingsListener: CreateEvent failed with error code {}", GetLastError());

        CloseHandle(file);
        WARN(stop_message);
        return;
    }

    uint8_t change_buf[4096];

    auto queue_read = [&]()
    {
        return ReadDirectoryChangesW(file, change_buf, sizeof(change_buf), FALSE,
            FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE, nullptr, &overlapped,
            nullptr);
    };

    BOOL success = queue_read();

    if (!success)
    {
        CloseHandle(overlapped.hEvent);
        CloseHandle(file);
        WARN("SettingsListener: ReadDirectoryChangesW failed with error code {}", GetLastError());
        WARN(stop_message);
        return;
    }

    bool reload_pending = false;
    steady_clock::time_point last_change;

    INFO("SettingsListener: watching for config updates...");

    while (success)
    {
        DWORD timeout = INFINITE;

        if (reload_pending)
        {
            const auto elapsed = steady_clock::now() - last_change;

            if (elapsed >= debounce_delay)
            {
                timeout = 0;
            }
            else
            {
                timeout = static_cast<DWORD>(
                    duration_cast<milliseconds>(debounce_delay - elapsed).count());
            }
        }

        const DWORD result = WaitForSingleObject(overlapped.hEvent, timeout);

        if (result == WAIT_TIMEOUT)
        {
            reload_pending = false;

            INFO("SettingsListener: Settings changed, reloading...");

            Settings::GetSingleton()->Load();

            continue;
        }

        if (result != WAIT_OBJECT_0)
        {
            WARN("SettingsListener: WaitForSingleObject failed with result {}", result);
            break;
        }

        DWORD bytes_transferred = 0;

        if (!GetOverlappedResult(file, &overlapped, &bytes_transferred, FALSE))
        {
            WARN("SettingsListener: GetOverlappedResult failed with error code {}", GetLastError());
            break;
        }

        auto* evt = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(change_buf);

        bool config_changed = false;

        for (;;)
        {
            const std::wstring name(evt->FileName, evt->FileNameLength / sizeof(WCHAR));

            const bool relevant_action = evt->Action == FILE_ACTION_MODIFIED ||
                                         evt->Action == FILE_ACTION_ADDED ||
                                         evt->Action == FILE_ACTION_RENAMED_NEW_NAME;

            if (relevant_action && _wcsicmp(name.c_str(), cfg_filename.c_str()) == 0)
            {
                config_changed = true;
            }

            if (!evt->NextEntryOffset)
                break;

            evt = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(
                reinterpret_cast<uint8_t*>(evt) + evt->NextEntryOffset);
        }

        // Queue the next read immediately so we don't miss changes
        // while waiting for the debounce timer.
        success = queue_read();

        if (!success)
        {
            WARN("SettingsListener: ReadDirectoryChangesW failed with error code {}",
                GetLastError());
            break;
        }

        if (config_changed)
        {
            reload_pending = true;
            last_change = steady_clock::now();
        }
    }

    INFO(stop_message);

    CloseHandle(overlapped.hEvent);
    CloseHandle(file);
}
