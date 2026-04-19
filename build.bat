@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
cl /EHsc /W4 ShellVault.cpp /Fe:ShellVault.exe /link ws2_32.lib
if %ERRORLEVEL% EQU 0 (
    echo Build successful!
    ShellVault.exe --help
) else (
    echo Build failed!
)