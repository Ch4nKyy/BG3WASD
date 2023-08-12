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

### Do once:

1. In the game settings, remove WASD from controlling the camera, then close the game.

2. Edit  
```C:\Users\xxx\AppData\Local\Larian Studios\Baldur's Gate 3\PlayerProfiles\Public\inputconfig_p1.json```  
with a text editor and add the following entries.  
```
{
   "CharacterMoveBackward" : [ "c:leftstick_yneg", "key:s" ],
   "CharacterMoveForward" : [ "c:leftstick_ypos", "key:w" ],
   "CharacterMoveLeft" : [ "c:leftstick_xneg", "key:a" ],
   "CharacterMoveRight" : [ "c:leftstick_xpos", "key:d" ]
}
```

> If you followed step 1, this file must already exist!
>
> If your game resets this file, then you messed up the syntax.
> 
> Editing json can be a bit fiddly if you are no techie.  
> Please look to it that the syntax is correct. A few hints:
>  * There should only be one pair of curly braces! They must be at the beginning and the end of the
>  file.
>  * There should be no weird symbols.
>  * There should be no tabs, but only spaces!
>  * There should be a comma after every entry, except for the last!

3. Install NativeModLoader from
[Achievement Enabler](https://www.nexusmods.com/baldursgate3/mods/668)
   * Only download Part-1-NativeModLoader
   * Put bink2w64.dll and bink2w64_original.dll in
 C:\Program Files (x86)\Steam\steamapps\common\Baldurs Gate 3\bin\
4. Optionally but recommended:
    * Install [AutoHotKey](https://www.autohotkey.com/)
    * In-game, bind camera controls to arrow keys

### Do once and every time there is an update of this mod

1. [Download](https://github.com/Ch4nKyy/BG3WASD/releases) BG3WASD.dll and place it inside `C:\Program Files (x86)\Steam\steamapps\common\Baldurs
Gate 3\bin\NativeMods\`  
   * If the folder does not exist, create it.
2. Optionally but recommended:
   * Download WASD_Toggle.ahk. It does not matter where you put it.

### Do every time you start the game:

1. Optionally but recommended:
    * Start WASD_Toggle.ahk (with AutoHotKey)
    * I highly recommend starting the ahk script with admin rights. Available in the rightclick
    menu. Because otherwise it won't work on some machines.
    * You can also automate this step! Just google how to start an AHK script on startup!

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
The mod files might need to be adapted.  
It is also possible that an update resets your input config json, so please check that again!  
Also, Steam or the Launcher might overwrite the NativeModLoader dll files, so you might need to
re-download them.

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
