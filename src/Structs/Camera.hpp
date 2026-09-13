#pragma once

#include "Vector3.hpp"

enum CameraModeFlags : uint32_t
{
    kCombat = 1 << 0,
    kUnk2 = 1 << 1,
    kTactical = 1 << 2,
    kUnk8 = 1 << 3,
    kUnk10 = 1 << 4,
    kUnk20 = 1 << 5,
    kUnk40 = 1 << 6,
    kUnk80 = 1 << 7,
    kMouseRotation = 1 << 8
};

struct Camera
{
    uint64_t unk00;
    uint64_t unk08;
    uint64_t unk10;
    Vector3 cameraRootPos;
    Vector3 desiredCameraRootPos;
    uint32_t unk34;
    uint32_t unk38;
    uint32_t unk3C;
    Vector3 unkCameraRootPosA;
    Vector3 unkCameraRootPosB;
    float currentZoomA;
    float currentZoomB;
    float desiredZoom;
    float float_64;
    float float_68;
    float float_6C;
    float fltmax_70;
    Vector3 cameraRotation;
    Vector3 prevCameraRotation;
    uint32_t unk8C;
    uint32_t unk90;
    uint32_t unk94;
    float horizontalPanDelta;
    float verticalPanDelta;
    float currentAngleDelta;
    float mouseRotationDelta;
    float zoomDelta;
    CameraModeFlags cameraModeFlags;
    float angle;
    uint32_t unkB4;
    uint32_t unkB8;
    uint32_t unkBC;
    uint32_t unkC0;
    float rotationSpeed;
    Vector3 cameraPos;
    float verticalOffset;
    float float_D4;
    float float_D8;
    float verticalOffsetNegated;
    float float_E0;
    float float_E4;
    float verticalOffset_E8;
    uint64_t unkF0;
    uint64_t unkF8;
    uint64_t unk100;
    uint64_t unk108;
    uint64_t unk110;
    uint64_t unk118;
    uint64_t unk120;
    uint64_t unk128;
    uint64_t unk130;
    uint32_t unk138;
    uint32_t unkZoom_13C;
    float timer_140;
    uint32_t unk144;
    uint64_t unk148;
    Vector3 cameraPos_150;
    float prevZoom_15C;
    float currentZoom_160;
    float currentPitch_164;
    uint64_t unk168[15];
    float unk1E0;
    bool bool_1E4;
    uint8_t unk1E5;
    uint16_t unk1E6;
    uint64_t unk1E8;
    uint8_t unk1F0;
    uint8_t unk1F1;
    uint16_t unk1F2;
    uint32_t unk1F4;
    uint64_t unk1F8[11];
    uint32_t unk250;
    uint32_t unk254;
};
static_assert(offsetof(Camera, unkF0) == 0xF0);
static_assert(offsetof(Camera, cameraRootPos) == 0x18);
static_assert(offsetof(Camera, desiredCameraRootPos) == 0x24);
static_assert(offsetof(Camera, cameraModeFlags) == 0xA8);
static_assert(offsetof(Camera, unkF8) == 0xF8);
static_assert(offsetof(Camera, unkZoom_13C) == 0x13C);
