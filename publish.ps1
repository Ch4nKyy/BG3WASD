Remove-Item $PSScriptRoot/build -Recurse -Force -ErrorAction:SilentlyContinue -Confirm:$False | Out-Null
& cmake -B $PSScriptRoot/build -S $PSScriptRoot --preset=REL -DPLUGIN_MODE:BOOL=TRUE
& cmake --build $PSScriptRoot/build --config Release

Remove-Item $PSScriptRoot\package\bin -Force -Recurse -ErrorAction:SilentlyContinue | Out-Null
Remove-Item $PSScriptRoot\package\*.zip -Force -Recurse -ErrorAction:SilentlyContinue | Out-Null
New-Item -ItemType Directory -Path $PSScriptRoot\package\bin | Out-Null
New-Item -ItemType Directory -Path $PSScriptRoot\package\bin\NativeMods | Out-Null
Copy-Item $PSScriptRoot\build\Release\* $PSScriptRoot\package\bin\NativeMods\
Copy-Item $PSScriptRoot\package\*.toml $PSScriptRoot\package\bin\NativeMods\
Compress-Archive -Path $PSScriptRoot\package\bin -DestinationPath $PSScriptRoot\package\BG3WASD.zip -Force
Remove-Item $PSScriptRoot\package\bin -Force -Recurse  -ErrorAction:SilentlyContinue | Out-Null
