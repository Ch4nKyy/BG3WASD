#Requires -Version 5

# args
param (
    [Parameter(Mandatory)][ValidateSet('SOURCEGEN', 'DISTRIBUTE')][string]$Mode,
    [string]$Version,
    [string]$Path,
    [string]$Project
)


$ErrorActionPreference = "Stop"

$Folder = $PSScriptRoot | Split-Path -Leaf
$SourceExt = @('.c', '.cc', '.cpp', '.cxx', '.h', '.hpp', '.hxx', '.inl', 'inc', '.ixx')
$ConfigExt = @('.ini', '.json', '.toml')
$DocsExt = @('.md')
$env:ScriptCulture = (Get-Culture).Name -eq 'zh-CN'


function L {
    param (
        [Parameter(Mandatory)][string]$en,
        [string]$zh = ''
    )
	
    process {
        if ($env:ScriptCulture -and $zh) {
            return $zh
        }
        else {
            return $en
        }
    }
}

function Resolve-Files {
    param (
        [Parameter(ValueFromPipeline)][string]$a_parent = $PSScriptRoot,
        [string[]]$a_directory = @('include', 'src', 'test')
    )
    
    process {
        Push-Location $PSScriptRoot
        $_generated = [System.Collections.ArrayList]::new()

        try {
            foreach ($directory in $a_directory) {
                if (!$env:RebuildInvoke) {
                    Write-Host "`t[$a_parent/$directory]"
                }

                Get-ChildItem "$a_parent/$directory" -Recurse -File -ErrorAction SilentlyContinue | Where-Object {
                    ($_.Extension -in ($SourceExt + $DocsExt)) -and 
                    ($_.Name -notmatch 'Plugin.h|Version.h')
                } | Resolve-Path -Relative | ForEach-Object {
                    if (!$env:RebuildInvoke) {
                        Write-Host "`t`t<$_>"
                    }
                    $_generated.Add("`n`t`"$($_.Substring(2) -replace '\\', '/')`"") | Out-Null
                }
            }               
            
            Get-ChildItem "$a_parent" -File -ErrorAction SilentlyContinue | Where-Object {
                ($_.Extension -in ($ConfigExt + $DocsExt)) -and 
                ($_.Name -notmatch 'cmake|vcpkg')
            } | Resolve-Path -Relative | ForEach-Object {
                if (!$env:RebuildInvoke) {
                    Write-Host "`t`t<$_>"
                }
                $_generated.Add("`n`t`"$($_.Substring(2) -replace '\\', '/')`"") | Out-Null
            }
        }
        finally {
            Pop-Location
        }

        return $_generated
    }
}


Write-Host "`n`t<$Folder> [$Mode]"


# @@SOURCEGEN
if ($Mode -eq 'SOURCEGEN') {
    Write-Host "`tGenerating CMake sourcelist..."
    Remove-Item "$Path/sourcelist.cmake" -Force -Confirm:$false -ErrorAction Ignore

    $generated = 'set(SOURCES'
    $generated += $PSScriptRoot | Resolve-Files
    if ($Path) {
        $generated += $Path | Resolve-Files
    }
    $generated += "`n)"
    [IO.File]::WriteAllText("$Path/sourcelist.cmake", $generated)
}


# @@DISTRIBUTE
if ($Mode -eq 'DISTRIBUTE') {
    # update script to every project
    Get-ChildItem "$PSScriptRoot/*/*" -Directory | Where-Object {
        $_.Name -notin @('vcpkg', 'Build', '.git', '.vs') -and
        (Test-Path "$_/CMakeLists.txt" -PathType Leaf)
    } | ForEach-Object {
        Write-Host "`tUpdated <$_>"
        Robocopy.exe "$PSScriptRoot" "$_" '!Update.ps1' /MT /NJS /NFL /NDL /NJH | Out-Null
    }
}
