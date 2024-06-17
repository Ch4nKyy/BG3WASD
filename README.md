# BG3 WASD Movement

A mod for Baldur's Gate 3 that allows direct character movement with WASD keys (or others).

Find it on [Nexusmods](https://www.nexusmods.com/baldursgate3/mods/781).

## Building

### Requirements

- [CMake](https://cmake.org/)
  - Add this to your `PATH`
- [PowerShell](https://github.com/PowerShell/PowerShell/releases/latest)
- [Vcpkg](https://github.com/microsoft/vcpkg)
  - Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing
  vcpkg
- [Visual Studio Community 2022](https://visualstudio.microsoft.com/)
  - Desktop development with C++
- [Baldur's Gate 3 Steam Distribution](https://store.steampowered.com/app/1086940/Baldurs_Gate_3/)
  - Add the environment variable `BG3PATH` with the value as path to game install folder
  - Add `BG3PATH2` if you have a secondary installation (E.g. one for Steam, one for GOG)
- [7zip](https://www.7-zip.org/)
  - Install to default dir
  
### Register Visual Studio as a Generator

- Open `x64 Native Tools Command Prompt`
- Run `cmake`
- Close the cmd window

### Building

```
git clone https://github.com/Ch4nKyy/BG3WASD.git
cd BG3WASD
git submodule init
git submodule update --remote
.\build-release.ps1
```

### Solution Generation (Optional)
If you want to generate a Visual Studio solution, run the following command:
```
.\generate-sln.ps1
```

> ***Note:*** *This will generate a `BG3WASD.sln` file in the **build** directory.*
