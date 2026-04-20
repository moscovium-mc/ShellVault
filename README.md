# SHELLVAULT - Shellcode Execution Framework
**v1.0.0** - powered by the VX-API

## Overview

**ShellVault** is a CLI shellcode execution framework featuring 31+ execution techniques sourced from the VX-API project. It provides security researchers, red team operators, and malware developers with a versatile tool for testing shellcode execution across various Windows API methods.

## Quick Start

### Build

```powershell
cd ShellcodeLoaderBuilder
.\build.bat
```

## Run

```
.\ShellVault.exe --help
```

## CLI Usage

### List all 31 execution techniques
ShellVault.exe --list

### Execute shellcode with default technique (CreateThread)
ShellVault.exe shellcode.bin

### Execute shellcode with specific technique ID
ShellVault.exe shellcode.bin 3

### Generate standalone C loader code
ShellVault.exe shellcode.bin --generate output.c

## Mode

Run without arguments for menu:

```
ShellVault.exe
```

## Requirements

- Windows 10/11 (x64)

- Visual Studio 2019 Build Tools (or later)

- MSVC C++ Compiler

## Building

Using build.bat (Recommended)

### Output Binary

After building, the executable is: ShellVault.exe

## License

See LICENSE file for details.

### Disclaimer

This tool is provided for educational and authorized security research purposes only. The author assumes no liability for any misuse or damage caused by this software.
