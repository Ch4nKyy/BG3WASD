#pragma once

#include <string>

enum RotationCursorType
{
    INVISIBLE,
    CENTERED,
    VANILLA,
};

inline RotationCursorType RotationCursorTypeFromString(const std::string& str)
{
    if (str == "vanilla")
        return RotationCursorType::VANILLA;

    if (str == "centered")
        return RotationCursorType::CENTERED;

    return RotationCursorType::INVISIBLE;
}
