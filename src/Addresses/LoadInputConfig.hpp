#pragma once

class LoadInputConfig
{
public:
    static int64_t Call(int64_t a1, char a2);
    static bool Prepare();
    static bool IsValid();

private:
    using tLoadInputConfig = int64_t (*)(int64_t a1, char a2);
    static inline tLoadInputConfig Func = nullptr;
};
