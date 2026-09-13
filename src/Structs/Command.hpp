#pragma once

struct Command
{
    uint32_t id;  // See CommandId.hpp for command ids.
    uint16_t device_id;
    uint8_t player_id;
    uint8_t event_kind;  // some category
    float current_value1;
    float current_value2;
    uint32_t current_pressed;
    float new_value1;
    float new_value2;
    uint32_t new_pressed;
    uint32_t unk_20;
    uint16_t unk_22;
    uint16_t unk_24;
    uint32_t unk_28;
};
static_assert(sizeof(Command) == 0x2C);

struct InputHandler;

struct DispatchInfo
{
    uint32_t command_id;
    uint32_t handler_category_mask;
    uint32_t flags;
    uint32_t unk_0C;
    InputHandler* captured_handlers[4];
    uint8_t unk_30[32];
    uint32_t input_mode_mask;
};

struct CommandEnvelope
{
    DispatchInfo* dispatch_info;
    int8_t player;  // or some kind of slot
    uint8_t unk_09[3];
    Command command;
};
static_assert(sizeof(CommandEnvelope) == 0x38);
