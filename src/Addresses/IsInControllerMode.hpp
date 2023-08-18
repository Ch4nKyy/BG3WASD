#pragma once

class IsInControllerMode
{
public:
    static bool Read();
    static bool Prepare();

private:
    static inline bool* is_in_controller_mode = nullptr;
};
