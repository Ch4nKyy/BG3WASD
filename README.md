# BG3 WASD Movement

A mod for Baldur's Gate 3 that allows direct character movement with WASD keys (or others).

Last updated for: BG3 Hotfix #3.  
Newer versions might work out of the box, but sometimes stuff breaks.  
Works with both Vulkan and DX11.

I highly recommend to also use my AutoHotKey script to toggle between WASD Camera movement
and WASD Character movement. Default toggle key is CapsLock. Shift+W is auto-run.

Use together with a camera mod like [BG3Cam](https://github.com/shalzuth/BG3Cam) for a pretty
immersive experience
and common (MMO) RPG controls.

## How to use

1. From [Releases](https://github.com/Ch4nKyy/BG3WASD/releases), download inputconfig_p1.json and
place it inside  
`C:\Users\xxx\AppData\Local\Larian Studios\Baldur's Gate 3\PlayerProfiles\Public\`
   * Warning: This overwrites any custom hotkeys you set. If you want instructions that preserve
   your hotkeys, but are a bit more complicated, see
   [Editing the inputconfig](#editing-the-inputconfig).
   * This config will put WASD on character movement and Arrow Keys on camera movement
   * If your keyboard is not QWERTY or QWERTZ, you might have to edit this file.

1. From [Releases](https://github.com/Ch4nKyy/BG3WASD/releases), download BG3WASD.dll and place it inside  
`C:\Program Files (x86)\Steam\steamapps\common\Baldurs
Gate 3\bin\NativeMods\`  
   * If the NativeMods folder does not exist, create it.

1. From [this mod](https://github.com/Ch4nKyy/BG3WASD/releases), download Part-1-NativeModLoader,
extract the zip file and place both bink2w64.dll and bink2w64_original.dll in  
`C:\Program Files (x86)\Steam\steamapps\common\Baldurs Gate 3\bin\`
   * When Windows prompts whether you want to overwrite the existing files, say yes.

1. Optionally but recommended:
    * Install [AutoHotKey](https://www.autohotkey.com/)
    * From [Releases](https://github.com/Ch4nKyy/BG3WASD/releases), download WASD_Toggle.ahk. It does not matter where you put it.
    * In-game, bind camera controls to only arrow keys, not WASD!
    * Every time you play the game, run WASD_Toggle.ahk as administrator by right clicking it. You
    see if it is running in the tray menu, bottom right.

## Hints

### Larian Launcher tells you that the there is a data mismatch

The launcher will tell you that files are corrupted.  
This is normal right now, since the mod launcher overwrites a dll file. Please ignore.

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

### The mod worked before, but stopped working!

Game updates can break the mod in multiple ways.  
The mod might need an update.  
It is also possible that a game update or file verification resets your inputconfig and overwrites
your modded files! Then you need to copy them again.

## Editing the inputconfig

If you don't want to overwrite the whole inputconfig_p1.json file, you need to manually open it with
a text editor and add the following content at the top, but after the `{` :

```
"CharacterMoveBackward" : [ "c:leftstick_ypos", "key:s" ],
"CharacterMoveForward" : [ "c:leftstick_yneg", "key:w" ],
"CharacterMoveLeft" : [ "c:leftstick_xneg", "key:a" ],
"CharacterMoveRight" : [ "c:leftstick_xpos", "key:d" ],
```

Also, you should unbind WASD from the Camera controls, otherwise the hotkeys will conflict and the
movement will be buggy.

But be careful, json syntax is very delicate.
 * There must only be one pair of curly braces! They must be at the beginning and the end of the
 file.
 * There should be a comma after every entry, except for the last!
 * There should be no weird symbols.

If inputconfig_p1.json does not exist yet, then you never modified the hotkeys and you can just copy
paste the provided file.

If your game resets the hotkeys or this file, after you edited it, then you messed up the syntax.

In the end, the file should look something like this:

```
{
  "CharacterMoveBackward" : [ "c:leftstick_yneg", "key:s" ],
  "CharacterMoveForward" : [ "c:leftstick_ypos", "key:w" ],
  "CharacterMoveLeft" : [ "c:leftstick_xneg", "key:a" ],
  "CharacterMoveRight" : [ "c:leftstick_xpos", "key:d" ],
  "CameraBackward" : [ "c:leftstick_ypos", "INVALID:unknown", "key:down" ],
  "CameraForward" : [ "c:leftstick_yneg", "INVALID:unknown", "key:up" ],
  "CameraLeft" : [ "c:leftstick_xneg", "INVALID:unknown", "key:left" ],
  "CameraRight" : [ "c:leftstick_xpos", "INVALID:unknown", "key:right" ]
}
```

# Building

This section is only needed by developers.  
If you just want to use the mod, see the section above.

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
