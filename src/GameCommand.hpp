#pragma once

enum GameCommand
{
    CameraToggleMouseRotateDONTUSE = 100, // Does not work yet with the default values
    CameraBackward = 101,
    CameraForward = 102,
    CameraLeft = 103,
    CameraRight = 104,
    CameraRotateDONTUSE = 108, // Use CameraToggleMouseRotate instead
    CharacterMoveForward = 141,
    CharacterMoveBackward = 142,
    CharacterMoveLeft = 143,
    CharacterMoveRight = 144,
    ActionCancel = 159,
    ContextMenu = 199,
};
