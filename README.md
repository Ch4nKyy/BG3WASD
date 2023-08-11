# BG3 WASD Movement

A mod for Baldur's Gate 3 that allows direct character movement with WASD keys (or others).

Last updated: BG3 v4.1.1.3635601.  
Newer versions might work, but sometimes stuff breaks.  
Works with both Vulkan and DX11.

I highly recommend to also use my AutoHotKey script to toggle between WASD Camera movement
and WASD Character movement. Default toggle key is CapsLock. Shift+W is auto-walk.

Use together with a camera mod like [BG3Cam](https://github.com/shalzuth/BG3Cam) for a pretty
immersive experience
and common (MMO) RPG controls.

## How to use

### Do once:

1. Edit  
```C:\Users\xxx\AppData\Local\Larian Studios\Baldur's Gate 3\PlayerProfiles\Public\inputconfig_p1.json```  
and add the following entries.  
(If the file does not exist yet, create a text file with this name (Be sure to not hide file
extensions in the explorer!) OR go in-game, change a hotkey and exit the game, then it will be
created for you.)
```
{
   "CharacterMoveBackward" : [ "c:leftstick_ypos", "key:s" ],
   "CharacterMoveForward" : [ "c:leftstick_yneg", "key:w" ],
   "CharacterMoveLeft" : [ "c:leftstick_xneg", "key:a" ],
   "CharacterMoveRight" : [ "c:leftstick_xpos", "key:d" ]
}
```
2. Install [Cheat Engine](https://www.cheatengine.org/)
3. Optionally but recommended:
    * Install [AutoHotKey](https://www.autohotkey.com/)
    * In-game, bind camera controls to arrow keys

### Do once and every time there is an update of this mod

1. On https://github.com/Ch4nKyy/BG3WASD, click ```Code``` and then ```Download ZIP```
2. Extract ZIP

Currently, it does not matter where you put these files, you have to start them manually anyway.

(This way I don't need to create releases all the time.)

### Do every time you start the game:

1. Open WASD.CT (with Cheat Engine)
2. In Cheat Engine, open the process Baldur's Gate 3 and keep the current code list.
3. Check the box "Apply WASD Patch". This takes a few seconds!  
(Please note that unchecking the box will not disable the mod, but restarting the game will.)
4. Optionally but recommended:
    * Start WASD_Toggle.ahk (with AutoHotKey)

## Hints

### Movement or camera gets stuck

If you press a camera key, while you already hold down a character
key, the game will bug and freeze the camera for a few seconds.  
This is very annoying. And it gets worse, when you bind the camera on Shift+WASD.
That is why I highly recommend to use my AutoHotKey Toggle script, so you toggle between camera and
character control and never press both at the
same time accidentally.  
The AutoHotKey script can be edited with a normal text editor.

### Cancel click move

When you click somewhere and your character auto-walks, then it will ignore your WASD inputs until
it reaches its destination or you press Cancel Action (by default, right click).

## Development

If you want to update the mod yourself, see /doc.
Requires some experience in reverse engineering.
