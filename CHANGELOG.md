# Changelog

## 0.1.0 (2026-11-03)

First release.

- Windows installer (recomp-launcher): pick the folder with your CarnEvil v1.0.1 files
  (MAME's `carnevil.zip` and `carnevi1.chd`); it builds the game from them and leaves a Play button.
  `Setup.cmd` / `tools/setup.ps1` does the same from a terminal.
- Boots from the boot ROM through the BIOS to `GAME.EXE`; attract mode, coin-up and full gameplay.
- One or two players on one machine (mouse and gamepad), or two over the network.
- Menus for video, sound, controls, cheats and multiplayer; operator settings kept in CMOS.
