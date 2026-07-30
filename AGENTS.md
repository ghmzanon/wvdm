# vdmanager — Lightweight Windows Virtual Desktop Switcher

Switches between Windows 10/11 virtual desktops using `Alt+1` through `Alt+9`.

## How it works

1. Registers 9 global hotkeys (`Alt+1..9`) via `RegisterHotKey(NULL, ...)`
2. Runs a `GetMessage` loop waiting for `WM_HOTKEY` events
3. On hotkey press, uses the undocumented COM interface `IVirtualDesktopManagerInternal`:
   - `CoCreateInstance(CLSID_ImmersiveShell)` → `IServiceProvider`
   - `QueryService(CLSID_VirtualDesktopManagerInternal)` → `IVirtualDesktopManagerInternal`
   - `GetDesktops()` → `IObjectArray`
   - `GetAt(index, IID_IVirtualDesktop)` → `IVirtualDesktop`
   - `SwitchDesktop(desktop)` → switches to the target desktop

## Build

### MSVC (Windows, recommended)

```
build.bat
```

Or manually:
```
cl /O1 /GS- main.c /link /SUBSYSTEM:WINDOWS kernel32.lib user32.lib ole32.lib
```

### MinGW cross-compile (Linux)

```
make CC=x86_64-w64-mingw32-gcc
```

## Run

Double-click `vdmanager.exe` or launch from terminal. No window appears — it runs as a background process. Press `Alt+1` through `Alt+9` to switch to desktop 1-9.

To stop: kill the process from Task Manager or `taskkill /IM vdmanager.exe`.

## Binary size

With MSVC `/O1 /GS-`: ~10 KB.
With MinGW `-O2 -Os -s`: ~15 KB.
