; Original script by Ch4nKyy
; Improved by Spanksh, thank you very much!

; Performance settings
#SingleInstance Force
#NoEnv
#MaxHotkeysPerInterval 99000000
#HotkeyInterval 99000000
#KeyHistory 0
#InstallKeybdHook
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.
ListLines Off
Process, Priority, , A
SetBatchLines, -1
SetDefaultMouseSpeed, 0
setKeyDelay -1, 0
SetMouseDelay, -1
SetWinDelay, -1
SetControlDelay, -1
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability. Default in AHK v2.
SetStoreCapsLockMode, Off ; If SetStoreCapsLockMode would be On, AHK would press CapsLock twice everytime you do a Send
SetTitleMatchMode 3 ; 3: Exact match
#MaxThreadsPerHotkey 1

; Variables =======================================================================================

bg3_RightClickShortOrHold_THRESHOLD := 0.100 ; seconds
bg3_StartHoldImmediately := true

; Game hotkeys ====================================================================================

bg3_ghk_Hold = MButton
bg3_ghk_Short = RButton

; Virtual hotkeys =================================================================================

bg3_vhk_RightClickShortOrHold = RButton

; Code ============================================================================================

GroupAdd, BG3Group, ahk_exe bg3.exe
GroupAdd, BG3Group, ahk_exe bg3_dx11.exe
Hotkey, IfWinActive, ahk_group BG3Group

Hotkey, *$%bg3_vhk_RightClickShortOrHold%, bg3_RightClickShortOrHoldDown

return

bg3_RightClickShortOrHoldDown:
	If %bg3_StartHoldImmediately% {
		Send {blind}{%bg3_ghk_Hold% down}
	}
	Keywait, %bg3_vhk_RightClickShortOrHold%, T%bg3_RightClickShortOrHold_THRESHOLD% ; Wait for click to be released
	If ErrorLevel  ; If it wasn't released within threshold
	{ 
		If not %bg3_StartHoldImmediately% {
			Send {blind}{%bg3_ghk_Hold% down}
		}
		Keywait, %bg3_vhk_RightClickShortOrHold% ; indefinitely wait for the release
		Send {blind}{%bg3_ghk_Hold% up}
	}
	Else ; If it was released within threshold
	{
		If %bg3_StartHoldImmediately%
		{
			Send {blind}{%bg3_ghk_Hold% up}
		}
		Send {blind}{%bg3_ghk_Short%}
	}
	return
