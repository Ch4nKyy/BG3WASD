# Test Checklist

Check these features before publishing:

- WASD Movement
  - Toggle WASD
  - Auto toggle works when you enter/leave combat and when you load a save that is in combat.
  - Toggle and Hold Walk
  - Toggle Autoforward
- Options in the toml are editable during gameplay without crashing the game.
- Rotation Cursor "Invisible"
  - Cursor hides and stays in place during rotation. Interact UI and behaviour is disabled.
    - After rotation, cursor icon is restored to the correct icon and not stuck on the rotate icon.
    - Also works when holding Alt or Shift or Ctrl meanwhile
- Rotation Cursor "Centered"
  - Cursor is a crosshair in the middle of the screen during rotation.
- ToggleCameraRotation
  - Toggling the rotation back and forth with the mod hotkey works, without getting the cursor stuck or anything.
  - Opening menus, dialog, etc, where the cursor is needed, always automatically toggles it off temporarily.
  - Tabbing in restores the correct toggle and cursor state
  - Toggling movement mode during rotation works and adapts to the respective cursor type.
  - TogglingMovementModeAlsoTogglesCameraRotation does not cause weird behaviour.
- Auto rotate is possible if enabled. It should not auto-enable itself on game start.
- PreventClickingActiveCharacter
- PreventClickingCameraOccluders
- BlockInteractMove
- Rotate+Leftclick = Forward
- Panning is disabled in character movement mode and does not cause the camera to lose focus on the target
- Rightclick Camera Rotate works with Cancel Action and Context Menu
  - Rightclick does not interrupt movement
  - Context Menu can be opened during WASD movement without delay
  - During interactmove, you can rotate the camera
  - During ability targeting, you can rotate the camera
  - These things sometimes fail when you just started the game, but after a few seconds and clicks it should work.
- Inputconfig
  - Keybindings are saved into the json, when you change a key in the menu and change to another tab
  - Keybinding check reports errors in toml (invalid keynames) and warn the user
  - No random errors/warnings should occur
  - Game successfully starts even without existing inputconfig
- Game does not crash, when you manually make the WASD Unlock fail
- Controller is unaffected (then splitscreen should be fine as well)
- Multiplayer is unaffected
- Both Vulkan and DX11 work
- Both GOG and Steam work
- To be completely safe: Manually search for ALL signatures and make sure they only find ONE location!
  The mod only throws errors when signatures could not be found at all or when invalid locations
  where matched, but it doesn't warn if a signature was found in multiple valid locations. The risk
  is very low, but not zero.
