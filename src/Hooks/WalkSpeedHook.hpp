#pragma once

struct Vector3
{
    float y, x;
};

class WalkSpeedHook
{
public:
    static void Enable();

private:
    static int64_t Hook_GetMovementInputVector(int64_t yx);
    static inline std::add_pointer_t<decltype(Hook_GetMovementInputVector)> _GetMovementInputVector;
};
