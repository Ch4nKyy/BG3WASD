
if (!$args) { throw "Provide version number as argument!" }
$version_number = $args[0]
if (-Not ($version_number -match '^\d\.\d\.\d$')) { throw "Version number must be of format x.y.z" }
$version_line = "    VERSION $version_number"
$file = "$PSScriptRoot\CMakeLists.txt"
$regex = '^\s*VERSION\s+\d\.\d\.\d$'
(Get-Content $file) -replace $regex, $version_line | Set-Content $file

Remove-Item $PSScriptRoot/build -Recurse -Force -ErrorAction:SilentlyContinue -Confirm:$False | Out-Null
& cmake -B $PSScriptRoot/build -S $PSScriptRoot --preset=REL -DPLUGIN_MODE:BOOL=TRUE
& cmake --build $PSScriptRoot/build --config Release

Remove-Item $PSScriptRoot\package\bin -Force -Recurse -ErrorAction:SilentlyContinue | Out-Null
Remove-Item $PSScriptRoot\package\*.zip -Force -Recurse -ErrorAction:SilentlyContinue | Out-Null
New-Item -ItemType Directory -Path $PSScriptRoot\package\bin | Out-Null
New-Item -ItemType Directory -Path $PSScriptRoot\package\bin\NativeMods | Out-Null
Copy-Item $PSScriptRoot\build\Release\* $PSScriptRoot\package\bin\NativeMods\
Copy-Item $PSScriptRoot\package\*.toml $PSScriptRoot\package\bin\NativeMods\
& "C:\Program Files\7-Zip\7z.exe" a -tzip $PSScriptRoot\package\BG3WASD.zip $PSScriptRoot\package\bin
Remove-Item $PSScriptRoot\package\bin -Force -Recurse  -ErrorAction:SilentlyContinue | Out-Null
