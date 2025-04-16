# Test Checklist

Check these features before publishing:

* WASD Movement
  * Toggle WASD
  * Auto toggle works when you enter/leave combat and when you load a save that is in combat.
* Toggle and Hold Walk
* Toggle Autoforward
* Game starts without existing inputconfig
* Features are deactivatable in the toml without crashing the game.
* Mouselook
  * Cursor hides and stays in place during rotation. Interact UI and behaviour is disabled.
    * After rotation, cursor icon is restored to the correct icon and not stuck on the rotate icon.
    * Also works when holding Alt or Shift or Ctrl meanwhile
  * Rotate+Leftclick = Forward
  * Rightclick Camera Rotate works with Cancel Action and Context Menu
    * (For some reason, the following points sometimes don't work after you tab in or start the
    game. But after a few seconds and clicking around, it should work!)
    * Context Menu opens after short click, but not after long click
    * Rightclick does not interrupt movement
    * Context Menu can be opened during WASD movement
* Keybindings are saved into the json, when you change a key in the menu and change to another tab
* Keybinding check reports errors in toml (invalid keynames) and game menu (important keys unbound)
* Game does not crash, when you manually make the WASD Unlock fail
* Controller is unaffected
* Both Vulkan and DX11 work
* Both GOG and Steam work
