#SingleInstance Ignore
#NoEnv
#MaxHotkeysPerInterval 99000000
#HotkeyInterval 99000000
#KeyHistory 0
#InstallKeybdHook
SetWorkingDir %A_ScriptDir%
ListLines Off
Process, Priority, , A
SetBatchLines, -1
setKeyDelay -1, 0
SetDefaultMouseSpeed, 0
SetMouseDelay, -1
SetWinDelay, -1
SetControlDelay, -1
SendMode Input
SetStoreCapsLockMode, Off
SetTitleMatchMode 3 ; 3: Exact match
#MaxThreadsPerHotkey 1

#Include <classMemory>

GroupAdd, BG3Group, ahk_exe bg3.exe
GroupAdd, BG3Group, ahk_exe bg3_dx11.exe

Tooltip, Loading AHK script...

if (_ClassMemory.__Class != "_ClassMemory")
{
    msgbox class memory not correctly installed. Or the (global class) variable "_ClassMemory" has been overwritten
    ExitApp
}

handle := new _ClassMemory("ahk_group BG3Group", "", hProcess)
if !isObject(handle)
{
    MsgBox, "Opening process failed. Game must be running!"
	ExitApp
}

pattern := handle.hexStringToPattern("062C934E")
addr := handle.processPatternScan(0x00, 0xFFFFFFFFFFFF, pattern*)
if addr < 0
{
    MsgBox, "AOB scan failed. Try to run script as admin!"
    ExitApp
}

Tooltip

bg3_WALKING := 0

Return

#IfWinActive ahk_group BG3Group

Insert::
    if (bg3_WALKING = 0)
    {
        handle.write(addr + 8, 5.0, "Float")
        bg3_WALKING = 1
    }
    else
    {
        handle.write(addr + 8, 1.0, "Float")
        bg3_WALKING = 0
    }
    return
