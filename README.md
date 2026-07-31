# wvdm — suckless Windows Virtual Desktop Manager

wvdm 1.0 — single-file C program (~510 LOC, ~6 KB PE32+) that switches
between Windows 10/11 virtual desktops, moves windows, creates/removes
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

Requires MinGW-w64 (`x86_64-w64-mingw32-gcc`).

```
make CC=x86_64-w64-mingw32-gcc
```

No configure step, no dependencies. Edit `config.h` before building to
customize.

## Configuration

All behavior toggles and key bindings live in `config.h`:

| Define | Default | Description |
|---|---|---|
| `SWITCH_AFTER_ACTION` | `1` | Set to `0` to stay on current desktop after move/create (removal always switches) |
| `WRAP_ADJACENT` | `0` | Set to `1` to wrap around on adjacent navigation at edges |
| `MOD_*` / `KEY_*` | | Key combination for each action |

Edit, rebuild, done.

## Run

Double-click `wvdm.exe` or launch from terminal. No window appears —
runs as a background process. Press `Ctrl+Alt+Q` to exit cleanly.

## License

MIT, see [LICENSE](LICENSE).
