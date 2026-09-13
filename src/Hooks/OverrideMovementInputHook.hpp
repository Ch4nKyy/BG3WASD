#pragma once

#include "Hooks/Base/FunctionHook.hpp"

struct Command;

// Caller: GetMovementInputWorldVec
// Target: GetInputValue
// Thread: GameThread
// Called when: movement is allowed, once per frame.
// Purpose: Override the input value for movement, allowing custom handling of player movement input.
class OverrideMovementInputHook : public FunctionHook
{
public:
    OverrideMovementInputHook() :
        FunctionHook(
            { search_pattern<"E8 ?? ?? ?? ?? 44 0F B6 CF C7 84 24 ?? ?? ?? ?? 9F 00 00 00">(),
                search_pattern<"E8 ?? ?? ?? ?? 44 0F B6 CF C7 84 24 ?? ?? ?? ?? 9D 00 00 00">(),
                search_pattern<"E8 ?? ?? ?? ?? 44 0F B6 CF C7 84 24 ?? ?? ?? ?? 9E 00 00 00">(),
                search_pattern<"48 8B CE 48 8D 54 24 ?? F3 44 0F 10 50 04 F3 44 0F 5C CA E8">() },
            { 0, 0, 0, 19 }, std::source_location::current().function_name()) {};

    void EnableSpecifically(intptr_t address_incl_offset) override;

    static int64_t OverrideFunc(int64_t player_input_controller_ptr, int64_t in_out_value_vec_ptr,
        Command* command, uint8_t player_id);
    static inline std::add_pointer_t<decltype(OverrideFunc)> OriginalFunc;

    static OverrideMovementInputHook& Get()
    {
        static OverrideMovementInputHook instance;
        return instance;
    }
};
