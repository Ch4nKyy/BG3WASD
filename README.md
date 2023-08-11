# BG3 WASD Movement

A mod for Baldur's Gate 3 that allows direct character movement with WASD keys (or others).

Last updated: BG3 v4.1.1.3635601.  
Newer versions might work, but sometimes stuff breaks.

Currently, only works with Vulkan, because I get better performance with it.  
If you desperately need DX11, create an issue or create the AOB scan yourself. See /doc.

I highly recommend to also use my AutoHotKey script to toggle between WASD Camera movement
and WASD Character movement. Default toggle key is CapsLock.

Use together with [BG3Cam](https://github.com/shalzuth/BG3Cam) for a pretty immersive experience
and common (MMO) RPG controls.

## How to use

### Do once:

1. Edit  
```C:\Users\xxx\AppData\Local\Larian Studios\Baldur's Gate 3\PlayerProfiles\Public\inputconfig_p1.json```  
and merge the following in there
```
{
   "CharacterMoveBackward" : [ "c:leftstick_ypos", "key:s" ],
   "CharacterMoveForward" : [ "c:leftstick_yneg", "key:w" ],
   "CharacterMoveLeft" : [ "c:leftstick_xneg", "key:a" ],
   "CharacterMoveRight" : [ "c:leftstick_xpos", "key:d" ]
}
```
2. Install NativeModLoader from [Achievement Enabler](https://www.nexusmods.com/baldursgate3/mods/668)
3. Optionally but recommended:
    * Install AutoHotKey
    * In-game, bind camera controls to arrow keys

### Do once and every time there is an update of this mod

1. Place BG3WASD.dll inside `Baldurs Gate 3\bin\NativeMods\`

### Do every time you start the game:

1. Optionally but recommended:
    * Start WASD_Toggle.ahk (with AutoHotKey)

## Hints

### Movement or camera gets stuck

If you press a camera key, while you already hold down a character
key, the game will bug and freeze the camera for a few seconds.  
This is very annoying. And it gets worse, when you bind the camera on Shift+WASD.
That is why I highly recommend to use my AutoHotKey Toggle script, so you toggle between camera and
character control and never press both at the
same time accidentally.  
The AutoHotKey script can be edited with a normal text editor.

### Cancel click move

When you click somewhere and your character auto-walks, then it will ignore your WASD inputs until
it reaches its destination or you press Cancel Action (by default, right click).

## Development

If you want to update the mod yourself, see /doc.
Requires some experience in reverse engineering.

# Building

## Requirements

- [CMake](https://cmake.org/)
  - Add this to your `PATH`
- [PowerShell](https://github.com/PowerShell/PowerShell/releases/latest)
- [Vcpkg](https://github.com/microsoft/vcpkg)
  - Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
- [Visual Studio Community 2022](https://visualstudio.microsoft.com/)
  - Desktop development with C++
- [Baldur's Gate 3 Steam Distribution](https://store.steampowered.com/app/1086940/Baldurs_Gate_3/)
  - Add the environment variable `BG3PATH` with the value as path to game install folder
  
## Register Visual Studio as a Generator

- Open `x64 Native Tools Command Prompt`
- Run `cmake`
- Close the cmd window

## Building

```
git clone https://github.com/Ch4nKyy/BG3WASD.git
cd BG3WASD
git submodule init
git submodule update
.\build-release.ps1
```

### Solution Generation (Optional)
If you want to generate a Visual Studio solution, run the following command:
```
.\generate-sln.ps1
```

> ***Note:*** *This will generate a `BG3WASD.sln` file in the **build** directory.*
