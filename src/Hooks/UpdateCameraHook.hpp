#pragma once

#include "Hooks/Base/FunctionHook.hpp"

class UpdateCameraHook : public FunctionHook
{
public:
    UpdateCameraHook() :
        FunctionHook({ search_pattern<
                         "E8 ?? ?? ?? ?? 48 8D 8D E8 04 00 00 E8 ?? ?? ?? ?? E9 A6 FD FF FF">() },
            { 0 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(uintptr_t address_incl_offset) override;

    static int64_t OverrideFunc(uint64_t a1, uint64_t a2, uint64_t a3, int64_t a4);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static UpdateCameraHook& Get()
    {
        static UpdateCameraHook instance;
        return instance;
    }
};
