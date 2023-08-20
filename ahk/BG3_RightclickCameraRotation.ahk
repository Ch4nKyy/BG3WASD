; Original script by Ch4nKyy
; Improved by Spanksh, thank you very much!

#SingleInstance Force

; Variables =======================================================================================

bg3_RightClickShortOrHold_THRESHOLD := 0.125 ; seconds
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
; This only exists because the Hotkey will sometimes turn off Alt when held while pressing Hotkey
Hotkey, *$Alt, bg3_RestoreAltFunction

return

bg3_RightClickShortOrHoldDown:
	If %bg3_StartHoldImmediately% {
		Send {%bg3_ghk_Hold% down}
	}
	Keywait, %bg3_vhk_RightClickShortOrHold%, T%bg3_RightClickShortOrHold_THRESHOLD% ; Wait for click to be released
	If ErrorLevel  ; If it wasn't released within threshold
	{ 
		If not %bg3_StartHoldImmediately% {
			Send {%bg3_ghk_Hold% down}
		}
		Keywait, %bg3_vhk_RightClickShortOrHold% ; indefinitely wait for the release
		Send {%bg3_ghk_Hold% up}
	}
	Else ; If it was released within threshold
	{
		If %bg3_StartHoldImmediately%
		{
			Send {%bg3_ghk_Hold% up}
		}
		Send {%bg3_ghk_Short%}
	}
	return

bg3_RestoreAltFunction:
	Send {Alt down}
	Keywait, Alt
	Send {Alt up}
	return
