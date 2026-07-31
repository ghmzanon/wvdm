# wvdm — suckless Windows Virtual Desktop Manager

Single-file C program (~510 LOC, ~6 KB PE32+) that switches between Windows
10/11 virtual desktops via global hotkeys. User-facing docs (hotkeys, build,
configuration, license) live in `README.md`.

## How it works

1. Registers global hotkeys via `RegisterHotKey(NULL, ...)`
2. Runs a `GetMessage` loop waiting for `WM_HOTKEY` events
3. Uses undocumented COM interfaces from `CLSID_ImmersiveShell`:
   - `IVirtualDesktopManagerInternal` — switching, moving, creating, removing
     desktops
   - `IApplicationViewCollection` — getting an application view from an HWND
     (for move operations)

## Build

```
make CC=x86_64-w64-mingw32-gcc
```

No configure step, no dependencies. Edit `config.h` before building to
customize.

## File layout

- `wvdm.c` — the whole program
- `config.h` — hotkey bindings and behavior toggles

Keep edits to `wvdm.c` / `config.h` only; rebuild with `make` after changes.

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
