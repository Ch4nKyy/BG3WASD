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

AUTOWALK := 0

CAMERA := 0
CHARACTER := 1
WASDControls := CHARACTER

GroupAdd, BG3Group, ahk_exe bg3.exe
GroupAdd, BG3Group, ahk_exe bg3_dx11.exe

Return

#IfWinActive ahk_group BG3Group

$CapsLock::
    Send {Up up}
    Send {Left up}
    Send {Down up}
    Send {Right up}
    Send {z up}
    Send {q up}
    Send {s up}
    Send {d up}
    if (WASDControls = CHARACTER)
    {
	    WASDControls := CAMERA
	}
    else
    {
        WASDControls := CHARACTER
    }
    return

$+z up::
    if (WASDControls = CAMERA)
    {
	}
    else
    {
        if (AUTOWALK = 0)
        {
            Send {z down}
            AUTOWALK = 1
        }
        Else
        {
            Send {z up}
            AUTOWALK = 0
        }
    }
    return

$z::
    if (WASDControls = CAMERA)
    {
	    Send {Up down}
	}
    else
    {
        Send {z down}
    }
    return

$z up::
    if (WASDControls = CAMERA)
    {
	    Send {Up up}
	}
    else
    {
        Send {z up}
        AUTOWALK = 0
    }
    return

$q::
    if (WASDControls = CAMERA)
    {
	    Send {Left down}
	}
    else
    {
        Send {q down}
    }
    return

$q up::
    if (WASDControls = CAMERA)
    {
	    Send {Left up}
	}
    else
    {
        Send {q up}
    }
    return

$s::
    if (WASDControls = CAMERA)
    {
	    Send {Down down}
	}
    else
    {
        Send {s down}
    }
    return

$s up::
    if (WASDControls = CAMERA)
    {
	    Send {Down up}
	}
    else
    {
        Send {s up}
        if (AUTOWALK = 1)
        {
            Send {z up}
            AUTOWALK = 0
        }
    }
    return

$d::
    if (WASDControls = CAMERA)
    {
	    Send {Right down}
	}
    else
    {
        Send {d down}
    }
    return

$d up::
    if (WASDControls = CAMERA)
    {
	    Send {Right up}
	}
    else
    {
        Send {d up}
    }
    return
