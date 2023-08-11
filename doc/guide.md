# Guide

## How to reverse engineer if the mod breaks after an update

Open bg3.exe in IDA and let it analyze it completely. This takes a while.

In Strings view, search CharacterMoveForward and double click it.

Go to the only xref of aCharactermovef.

Go to Pseudocode view.

CharacterMoveForward is given to a function as parameter. This function also takes an int that is
declared in the line above. Should be 142, if it doesn't change. This is 0x8E or 8Eh in hex.

Search the following sequence of bytes. Tt should take you to the function that calculates the
movement input.  
```C7 ?? ?? ?? 90 00 00 00 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? C7 ?? ?? ?? 8E 00 00 00 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? C7 ?? ?? ?? 8F 00 00 00```  
If this doesn't find anything, text search for ```" 8Eh"``` or whatever hex value CharacterMoveForward is
and look for an occurrence where value, value+1, value+2 and value+3 are very close to each other!
These are the 4 command IDs for the movement input.

Rename this function GetMovementInputVec.  
Hint: The subfunction that takes "CharacterMoveForward" and the command ID, also takes the
PlayerController as its first parameter.

Set a breakpoint where this function is called.

Start the game with the debugger or attach the debugger to it.

Go into Controller Mode by pressing a controller button or through the menu.

The breakpoint should now be hit. Open the Call Stack view and copy all into a text file.
For me, the Call stack looks like this:  
    GetMovementInputVec  
    <- GetMovementInputWorldVec  
    <- ReactToControllerInputStuff  
    <- ReactToGeneralInputStuff  
Rename the functions like this!

Disable the breakpoint, resume execution and go into Keyboard mode by pressing a key in-game.
If you re-enable the breakpoint, it will not be hit anymore.

The strategy now is to place breakpoints in the functions higher on the stack trace and see which
one gets hit, so we find what is different between Keyboard mode and Controller mode.
So copy the address of the next higher function from your Call stack text file, jump to it and
place a breakpoint. Repeat until hit.

You will probably see that in Keyboard mode, you hit ReactToGeneralInputStuff.
You even enter ReactToControllerInputStuff, even if the Call stack doesn't show!
Just place a breakpoint at the top of this function.
However, notice that you exit the function very early, when doing some ```if (!byte_xxxx)```
comparison.
This byte is a bool whether controller mode is enabled or not.
Rename this byte to IsInControllerMode.
So we have to disable exactly this check.

There is a second check that must be disabled.
GetMovementInputWorldVec is called 4 times. One time we already investigated.
Check the other 3 times. In one of them, the function makes a lot of checks before and returns 1 in
them.
You will see that one of those checks is another ```if (!IsInControllerMode)```, so it has to be
disabled!

To disable them, we can create AOB scans for them by looking at their bytes.
We want to override the cmp and the jz (same as je) instruction with nops.
This is done in the Cheat Engine scripts.
