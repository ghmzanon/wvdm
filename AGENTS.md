# vdmanager — suckless Virtual Desktop Manager for Windows

Single-file C program (~510 LOC, ~6 KB PE32+) that switches between Windows 10/11 virtual desktops, moves windows, creates/removes desktops, and toggles last desktop — all via global hotkeys. Links only `kernel32`, `user32`, `ole32`. Zero dependencies, no CRT, no runtime config, no manifest, no external DLLs.

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
| `Ctrl+Alt+Space` | Toggle last desktop |
| `Ctrl+Alt+N` | Create new virtual desktop |
| `Ctrl+Alt+W` | Close current virtual desktop |
| `Ctrl+Alt+Q` | Quit |

## Build

```
make CC=x86_64-w64-mingw32-gcc
```

No configure step, no dependencies. Edit `config.h` before building to customize.

## Configuration

All behavior toggles and key bindings live in `config.h`:

| Define | Default | Description |
|---|---|---|
| `SWITCH_AFTER_ACTION` | `1` | Set to `0` to stay on current desktop after move/create (removal always switches) |
| `WRAP_ADJACENT` | `0` | Set to `1` to wrap around on adjacent navigation at edges |
| `MOD_*` / `KEY_*` | | Key combination for each action |

Edit, rebuild, done.

## Run

Double-click `vdmanager.exe` or launch from terminal. No window appears — runs as a background process. Press `Ctrl+Alt+Q` to exit cleanly.

## Styleguide

- **Braces**: always use braces for `if`/`for`/`while` bodies, even single-statement
  ones. Never `if (cond) return;` — expand to `if (cond) { return; }`.
- **Line limit**: 80 columns. Wrap long calls/signatures by breaking before the
  final argument and indenting the continuation line with tabs to align with the
  opening `(` (e.g. `WinMain`, `CoCreateInstance`, long vtable entries).
- **Method signatures**:
  - Opening brace `{` goes on the same line as the signature.
  - No-arg functions use `(void)`.
  - COM vtables: align return types to a column, then
    `(STDMETHODCALLTYPE *Name)(Type*, ...)`; wrap continuation args one tab in.
- **Comments**: use `/* */` (never `//`). Comment only *why* — undocumented
  interfaces, non-obvious version differences, vtable slot layouts and unused
  slots, and inline actual-COM-signature annotations (e.g.
  `/* RemoveDesktop(this, fallbackDesktop, desktopToRemove) */`). Never comment
  on code that is self-evident.
- **Column alignment**: align columns in declaration blocks (e.g. `static` globals:
  types padded so `*` and variable names line up) and in aligned text blocks
  (e.g. `print_help` strings: shortcut + padding so descriptions start at the
  same column).
- **Tabs** for indentation, spaces for intra-line alignment.
- Keep edits to `main.c` / `config.h` only; rebuild with `make` after changes.


