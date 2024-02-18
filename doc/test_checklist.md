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
  * Rotate+Leftclick = Forward
  * Rightclick Camera Rotate works with Cancel Action and Context Menu
    * Context Menu opens on click up, not click down
    * Context Menu does not open after a long click
    * Context Menu can be opened quickly after movement
* Keybinding check reports errors in toml (invalid keynames) and game menu (important keys unbound)
* Game does not crash, when you manually make the WASD Unlock fail
* Controller is unaffected
* Both Vulkan and DX11 work
