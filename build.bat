@echo off
cl /O1 /GS- /utf-8 main.c /link /SUBSYSTEM:WINDOWS kernel32.lib user32.lib ole32.lib
if %errorlevel% equ 0 (
    echo.
    echo Build successful: vdmanager.exe
    dir vdmanager.exe
)
