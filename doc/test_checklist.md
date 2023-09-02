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
  * Cursor hides and stays in place. Interact is disabled.
  * Rotate+Leftclick = Forward
  * Rightclick Camera Rotate works with Cancel Action and Context Menu
* Keybinding check reports errors in toml (invalid keynames) and game menu (important keys unbound)
* Game does not crash, when you manually make the WASD Unlock fail
* Controller is unaffected
* Both Vulkan and DX11 work
