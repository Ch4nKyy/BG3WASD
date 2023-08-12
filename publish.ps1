if (!(Test-Path $PSScriptRoot\build\Release\*)) {
    exit
}

Remove-Item $PSScriptRoot\NativeMods -Force -Recurse -ErrorAction:SilentlyContinue | Out-Null
Remove-Item $PSScriptRoot\*.zip -Force -Recurse -ErrorAction:SilentlyContinue | Out-Null
New-Item -ItemType Directory -Path $PSScriptRoot\NativeMods | Out-Null
Copy-Item $PSScriptRoot\build\Release\* $PSScriptRoot\NativeMods\
Compress-Archive -Path $PSScriptRoot\NativeMods\ -DestinationPath $PSScriptRoot\BG3WASD.zip -Force
Remove-Item $PSScriptRoot\NativeMods -Force -Recurse  -ErrorAction:SilentlyContinue | Out-Null