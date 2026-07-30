# vdmanager — Lightweight Windows Virtual Desktop Switcher

Switches between Windows 10/11 virtual desktops, moves windows between desktops, creates/removes desktops, all via global hotkeys.

## How it works

1. Registers global hotkeys via `RegisterHotKey(NULL, ...)`
2. Runs a `GetMessage` loop waiting for `WM_HOTKEY` events
3. Uses undocumented COM interfaces from `CLSID_ImmersiveShell`:
   - `IVirtualDesktopManagerInternal` — switching, moving, creating, removing desktops
   - `IApplicationViewCollection` — getting an application view from an HWND (for move operations)

## Hotkeys

| Shortcut | Action |
|---|---|
| `Alt+1..9` | Switch to desktop N |
| `Alt+Shift+1..9` | Move window + switch to desktop N |
| `Ctrl+Alt+Left/Right` | Previous/next desktop |
| `Ctrl+Alt+Shift+Left/Right` | Move window + switch to adjacent |
| `Ctrl+Alt+N` | Create new virtual desktop |
| `Ctrl+Alt+W` | Close current virtual desktop |
| `Ctrl+Alt+Q` | Quit |

## Build

```
make CC=x86_64-w64-mingw32-gcc
```

To customize hotkeys, edit `config.h` before building.

## Configuration

All key bindings are in `config.h` — edit and rebuild.

## Run

Double-click `vdmanager.exe` or launch from terminal. No window appears — runs as a background process. Press `Ctrl+Alt+Q` to exit cleanly.


