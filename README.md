# wvdm — suckless Windows Virtual Desktop Manager

wvdm 1.0 — single-file C program that switches
between Windows 11 virtual desktops, moves windows, creates/removes
desktops, and toggles last desktop — all via global hotkeys. Links only
`kernel32`, `user32`, `ole32`. Zero dependencies, no CRT, no runtime config,
no manifest, no external DLLs.

## Features

- Switch to any desktop with `Alt+1..9`
- Move the active window and switch in one step
- Adjacent desktop navigation with wrap-around option
- Toggle back to the last visited desktop
- Create and close virtual desktops
- Runs as a background process, no window

## Hotkeys

| Shortcut | Action |
|---|---|
| `Alt+1..9` | Switch to desktop N |
| `Alt+Shift+1..9` | Move window + switch to desktop N |
| `Ctrl+Alt+Left/Right` | Previous/next desktop |
| `Ctrl+Alt+Shift+Left/Right` | Move window + switch to adjacent |
| `Ctrl+Alt+Space` | Toggle last desktop |
| `Ctrl+Alt+N` | Create new virtual desktop |
| `Ctrl+Alt+W` | Close current virtual desktop |
| `Ctrl+Alt+Q` | Quit |

## Build

Dependencies: `make` and a MinGW-w64 cross-compiler (`x86_64-w64-mingw32-gcc`).

| Distro | Install |
|---|---|
| Debian/Ubuntu | `sudo apt install gcc-mingw-w64-x86-64 make` |
| Fedora | `sudo dnf install mingw64-gcc make` |
| Arch | `sudo pacman -S mingw-w64-gcc make` |

Then:

```
make
```

Copy the single `wvdm.exe` to the Windows machine — no DLLs or other files
needed. Edit `config.h` before building to customize.

Building from Windows works too: run the Linux steps inside WSL2, then grab
`wvdm.exe` from the WSL filesystem at `\\wsl$\<distro>\<path>`.

## Configuration

All behavior toggles and key bindings live in `config.h`:

| Define | Default | Description |
|---|---|---|
| `SWITCH_AFTER_ACTION` | `1` | Set to `0` to stay on current desktop after move/create (removal always switches) |
| `WRAP_ADJACENT` | `0` | Set to `1` to wrap around on adjacent navigation at edges |
| `MAX_DESKTOPS` | `9` | Numbered-desktop hotkeys; desktop creation stops at this cap |
| `MOD_*` / `KEY_*` | | Key combination for each action |

Edit, rebuild, done.

## Run

Double-click `wvdm.exe` or launch from terminal. No window appears —
runs as a background process. Press `Ctrl+Alt+Q` to exit cleanly. Only one
instance runs at a time — a second launch exits silently.

## Autostart

Windows runs anything in the Startup folder at logon. Just copy the exe there:

1. `Win+R`, type `shell:startup`, press Enter
2. Copy `wvdm.exe` (or a shortcut to it) into the folder

No need for `shell:startup`: the folder has a plain path, so a terminal works:

| Shell | Command |
|---|---|
| cmd | `copy wvdm.exe "%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup\"` |
| PowerShell | `Copy-Item wvdm.exe "$env:APPDATA\Microsoft\Windows\Start Menu\Programs\Startup\"` |

To uninstall: delete the file from the folder.

## License

MIT, see [LICENSE](LICENSE).
