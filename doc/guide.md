# Reverse Engineering Guide

Disclaimer: This is by no means an easy, step by step guide. It requires some foreknowledge.

Reversing a new BG3 binary, after a game update is released, is waaay easier, when you
already have an IDA Database for an old version of the game where the mod still works.
Because then you can search for the existing AOB patterns of the mod in the old Database and maybe
find striking code patterns or peculiarities that you can use to find this code location in the
new binary.  
I think, it should be possible on both GOG and Steam to get old game versions, although it is not
trivial.

In this guide, I discover and give names to functions chapter by chapter. So if one chapter
references some function you don't know, just search for the name in the whole guide and the
first occurrence should probably describe how to discover this function.  
You can also search it in input_program_flow.drawio.svg to get an overview.

Sometimes, the most robust technique to find a function is to
1. Set a breakpoint in a general function like CheckCommandsInput or HandleInput
1. Start the game with the debugger and stimulate it, e.g. by pressing the Interact button
1. Analyze the stack trace or even create a function trace until you end up in the desired function

The input_program_flow.drawio.svg can help navigating.

Often, I provide a short and a long way.  
The short way is easier and quicker to do, but might break due to significant code changes in the
future.  
The long way is more complicated and requires debugging and tracing, but is more robust
and might be needed if the short way breaks.

Useful links:  
https://defuse.ca/online-x86-assembler.htm#disassembly
https://stackoverflow.com/questions/54499327/setting-and-clearing-the-zero-flag-in-x86
https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention?view=msvc-170

## WASDUnlockPatch and GetInputValueHook

Open bg3.exe in IDA and let it analyze it completely. This takes a while.

In Strings view, search CharacterMoveForward and double click it.

Go to the only xref of aCharactermovef.

Go to Pseudocode view.

CharacterMoveForward is given to a function as parameter. This function also takes an int that is
declared in the line above. Should be 157, if it doesn't change. This is 0x9D or 9Dh in hex.

Hint: The subfunction that takes "CharacterMoveForward" and the command ID, also takes the
PlayerController as its first parameter.

Do a full text search for ```, 9Dh``` or whatever hex value CharacterMoveForward is
and look for an occurrence where value, value+1, value+2 and value+3 are very close to each other
assigned to variables or moved into registers!
There are 2 functions where this is the case. I name them GetMovementInputVec and
GetMovementInputWorldVec. The latter one has some matrix transformation math at the end.
This is the one we are looking for. Rename and mark this function, so we can easily find it.

In this function, 157/158/159/160 are assigned to variables. These variables are then passed to
a function I name GetInputValue. All 4 calls of this function calls are hooked by the mod!
So if one of those hooks breaks, you know how to repair it.

Now, there are 2 ways.

### Short way

The function GetMovementInputWorldVec is called in 3 xrefs. The third one should be a function I
call UpdateWasdMove.  
The UpdateWasdMove function starts with a check: if(!sub_xxx()) return 1.
Inside this check function sub_xxx, a lot of checks are done and one of them needs to be removed
for the WASD Unlock.

There is one check that goes like: if (!byte_xxx) return 0.
This byte can be called IsInControllerMode and this check must be nopped, specifically the cmp and
jz instructions.

### Long way

Disclaimer: I did not use/check the old way for a long time. Some steps might not be up to date
anymore.

Set a breakpoint where GetMovementInputWorldVec is called.

Start the game with the debugger or attach the debugger to it.

Go into Controller Mode by pressing a controller button or through the menu.

The breakpoint should now be hit. Open the Call Stack view and copy all into a text file.
For me, the Call stack looks like this:  
    GetMovementInputWorldVec  
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

To disable them, we can create AOB scans for them by looking at their bytes.
We want to override the cmp and the jz (same as je) instruction with nops.

## BlockCameraInputCavehook

To find this, you can check the xrefs of GetInputValue.
There should be a code section where GetInputValue is called 4 times next to each other with the
command ids 99, 100, 101, 102 (third parameter). These are the CameraForward/Backward/Left/Right
command IDs.  
I name this function HandleCameraInput.  
Somewhere at the top, there is a line `x = *(_QWORD *)(a3 + 64);`. x is the camera object.
Somewhere in the middle of HandleCameraInput, there is:  
```
*(_BYTE *)(CameraObject + 169) |= 4u;
*(_DWORD *)(CameraObject + 92) = *(_DWORD *)(CameraObject + 352);
*(_BYTE *)(CameraObject + 324) = 1;
```

(CameraObject + 324) is "should_move" and must be set to 0 in CharacterMovementMode.

## LoadInputConfig

Search for the string ```inputremap_p1.json```. It should only have one xref and that is inside the
function I name LoadInputConfig.  
The AOB signature aims at the function start, not at any call of the function.

## UpdateCameraHook

TODO

## AfterInitialLoadInputConfigHook

Unfortunately, LoadInputConfig is only called virtually, so we can't statically find the initial
call of it that we want to hook. But we don't hook the exact call anyway, we just hook SOME
function AFTER the initial call.  
To find this location, you can search for the String ```-saveStoryState```. It should only have one
xref.  
Inside this function, we want to hook ANY function call at the bottom.  
Currently, the call looks something like this:
```sub_xxx(a1 + 20, *((unsigned __int16 *)a1 + 143));```

If this does not work anymore, place a breakpoint inside LoadInputConfig and when it is hit the
very first time during game start, trace the stack trace.

## AfterChangingKeybindInMenuHook

### Short way

The string ls.VMListButton once helped me find it. Doesn't appear too often. The last occurrence is
in a function I name AfterChangingKeybindInMenu2. It is called 3 times. One of those calls we want
to hook.

### Long way

There is a function that maps keys from ints to strings. (Don't mistake it for the one that maps
strings to ints.) This function can easily be found by e.g. searching for the string "printscreen".
To find the function that writes the inputconfigs, just place a breakpoint in this mapping
function and look at the stack trace. We need to hook a spot that is reached AFTER the config file
has been written.

## BlockAnalogStickSelectionPatch

Search for the string ```hec9cb611gdbebg4618ga7c8g069215436e27```. It should only have 1 xref and
lead you to the function I name ChangeSetting.
At the very bottom, there is something like  
```
case aaa:
  *(_DWORD *)(qword_xxx + 3696) = zzz;
```

case aaa is the second to last case.
qword_xxx is the Settings ptr.
yyy should be the Analog Stick Selection setting.
You can cross-check by setting a breakpoint and debugging it and changing the setting.
The patch nops this assignment. The assembly looks something like this:

```
mov     rax, cs:Settings
mov     [rax+0E70h], ebx
```

## FTBStartHook/FTBEndHook

Search for the strings ```ftb_start``` or ```ftb_end```.
Hook the function call that is passed this string.

## PollEventHook

Search for the import "SDL_PollEvent". This function is called twice in the function I name
PollInput. We hook the first occurrence. It is at the top of the function.

## SDL_GetWindowGrabHook

Search for the import "SDL_PollEvent". This function is called twice in the function I name
PollInput.  
In the PollInput function, also SDL_GetWindowGrabHook is called twice. We hook the first call.

## SetCursorRotateHook

In the middle of the HandleCameraInput function, there is a case handler for case 98 (id of
CameraToggleMouseRotate). In this handler, there is a function that is called, depending on a
condition ```if ( (v44 & 1) != 0 )```. We hook the call in the true case.

## ResetCursorRotateHook

Same location as the SetCursorRotateHook, but in the false case of the conditional.
After that, hook ALL xrefs calls to this function!

## CheckCommandInputsHook

Place a breakpoint in HandleCameraInput.
When hit, analyze the stack trace.
At the time of writing, the CheckCommandInputs function is the 7th level caller.
The call position is almost at the bottom of the function and looks like this:
```
  for ( *(_DWORD *)(v4 + 844) = 0; v128 < v127; ++v128 )
  {
    v129 = *(_QWORD *)(v4 + 744);
    LOBYTE(v144) = 0;
    v151 = 0;
    v148.m128d_f64[0] = *(double *)(v129 + 8 * v128);
    v150 = v144;
    v149 = _mm_unpacklo_pd(v148, (__m128d)_mm_unpacklo_ps((__m128)0i64, (__m128)0i64));
    result = sub_7FF70283C6D0(v4, &v149);
    v4 = a1;
  }
```

Then, go one caller higher. It looks almost like a main loop with GetCurrentThreadId,
QueryPerformanceCounter, SetEvent and many function calls in a loop.
There, hook the call of CheckCommandInputs.

## CheckContextMenuOrCancelActionHook

### Short way

IsInControllerMode xrefs can be used to find this. The order of the refs are always very similar.  
Generally, you look for a function that has a case for 214 (ContextMenu) and 174 (ActionCancel),
but sometimes the compiler does not generate a switch statement.  
This function uses IsInControllerMode three times.

Then, hook the only call of this function.

### Long way

Debug, press ContextMenu and then trace all the way from CheckCommandInputs.

## DecideMoveUpdaterHook

### Short way

Use the AOB pattern ```1C 00 00 80 BF```. 1C is an offset. 00 00 80 BF or 0BF800000h is a constant
that is used in some movement functions. You should get like 10 hits.
You look for a function that does this:

```
    else
    {
      *(_DWORD *)(a1 + 28) = -1082130432;
      *(_QWORD *)(a1 + 64) = 0xFFC0000000000000ui64;
      *(_DWORD *)(a1 + 32) = 0;
    }
    if ( v14 )
      sub_xxx(a1, a2);
    *(_BYTE *)(a1 + 97) = 0;
    sub_yyy(a1, a2);
```

This is the DecideMoveUpdaterPre function.  
Be careful that the offsets and values are correct, because the 10 functions look very similarly.

sub_yyy is the call to DecideMoveUpdater that we hook.

### Long way

Use input_program_flow.drawio.svg as reference:  
Use the known UpdateWasdMove function to find DecideMoveUpdater by debugging and tracing.  
Then set a breakpoint in DecideMoveUpdater and do an InteractMove and trace again to find the call.

## InsideUpdateInteractMoveCavehook

### Short way

Use the AOB pattern ```BB 00 7D 00 00```. This should only have 1 hit in UpdateInteractmove.  
We want to hook ANY function in here that is always executed.  
Since Patch 4 this is not possible anymore, so we need a cavehook.

Alternatively, search for the string ```h2540f096gfe81g4931gb48bgcae2bb739b97```.  
It is referenced once. Inside this function, at the very top a function is called.  
```if ( !sub_xxxxxx(a1) || !*(_BYTE *)(a1 + 609) )```  
This function is UpdateInteractmove.

### Long way

Use input_program_flow.drawio.svg as reference:  
Use the known UpdateWasdMove function to find DecideMoveUpdater by debugging and tracing.  
Then set a breakpoint in DecideMoveUpdater and do an InteractMove and trace again to find the call.

## CallSpecificCommandFunctionPre2Cavehook

### Short way

Use the AOB pattern ```48 8D 54 24 48 48 8B C8 E8 ?? ?? ?? ?? 0F B7 00```.  
This finds the function CallSpecificCommandFunctionPre3.
TODO

### Long way

Use input_program_flow.drawio.svg as reference.  
Debug, press any command and then trace all the way from CheckCommandInputs.

## BlockInteractMovePatch

### Short way

Use the AOB pattern ```F6 80 5C 01 00 00 01```. This should have 3 hits.  
One of these functions, accesses a1+104 and checks if a3==4.  
This function is HandleInteractmove.  
Prevent entering this true if branch to block interact moves. E.g. by nopping jumps or 
overwriting compares.

### Long way

Use input_program_flow.drawio.svg as reference.  
Debug, press Interact and then trace all the way from CheckCommandInputs.

## BlockHoldInteractMovePatch

### Short way

Use the AOB pattern ```C7 46 1C 00 00 80 BF```.  
This has 2 hits, both in the function HandleMoveInput.  
In there, there is one line like ```*((_DWORD *)a1 + 8) = 256;```  
Nop this to block hold interact moves.  
In DOS2, it was ```*((_BYTE *)a1 + 33) = 1;```.

### Long way

Use input_program_flow.drawio.svg as reference.  
Debug, hold Interact and then trace all the way from CheckCommandInputs.

## CastOrCancelAbilityHook

I found it through the Settings ptr. The order of xrefs is almost identical, even for
different binaries. The Settings ptr is referenced 3 times in this function and put into rcx.

In the middle of this function, there is a call to a subfunction with many parameters, the last
being the Settings ptr qword, as discovered in BlockAnalogStickSelectionPatch.  
This subfunction I name CastOrCancelAbility and this call must be hooked.

## GameInputManager Class

In CallSpecificCommandFunctionPre3, virtual CallSpecificCommandFunctionPre2 is called.
Compare the signature. Note that in IDA, it will look like it has 1 parameter more (self).

## SetVirtualCursorPosHook and SetVirtualCursorPosFakeClass

In PollEvents, virtual SetVirtualCursorPos is called at the bottom.
Compare the signature. Note that in IDA, it will look like it has 1 parameter more (self).

## Reminder

A general reminder about things that can change in game patches and cause bugs without throwing
warnings:

The GameCommand IDs sometimes change! Make sure the IDs in GameCommand.hpp are in sync with the
game.

Offsets sometimes change! E.g. the Settings members or the CameraObject members.
