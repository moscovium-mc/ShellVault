
# SHELLVAULT - Shellcode Execution Framework
v1.0.0 - powered by the VX-API

```
## Overview

  

**ShellVault** is a cli shellcode execution framework featuring 31+ execution techniques sourced from the VX-API project. It provides security researchers, red team operators, and malware developers with a versatile tool for testing shellcode execution across various Windows API methods.

  

## Quick Start

  

### Build

  

```powershell

cd ShellcodeLoaderBuilder

.\build.bat

```

  

### Run

  

```powershell

.\ShellVault.exe  --help

```

  

## Command-Line Usage

  

```bash

# List all 31 execution techniques

ShellVault.exe  --list

  

# Execute shellcode with default technique (CreateThread)

ShellVault.exe  shellcode.bin

  

# Execute shellcode with specific technique ID

ShellVault.exe  shellcode.bin  3

  

# Generate standalone C loader code

ShellVault.exe  shellcode.bin  --generate  output.c

```

  

## Mode

  

Run without arguments for interactive menu:

  

```bash

ShellVault.exe

```

  

Menu options:

1. Select Technique

2. Load Shellcode File

3. Generate Loader Code

4. Execute Shellcode

5. List Techniques

0. Exit

  

## Technique ID Reference

  

| ID | Technique | Description | DLL |

|----|----------|------------|-----|

| 1 | CreateThread | Basic CreateThread execution |

| 2 | CreateRemoteThread | CreateRemoteThread injection |

| 3 | QueueUserAPC | APC QueueUserAPC injection |

| 4 | ThreadpoolTimer | Threadpool timer execution |

| 5 | ThreadpoolWork | Threadpool work item |

| 6 | ThreadpoolWait | Threadpool wait |

| 7 | TimerQueueTimer | Timer queue timer |

| 8 | TimerQueueTimerEx | Timer queue timer ex |

| 9 | RtlCreateUserThread | RtlCreateUserThread |

| 10 | NtQueueApcThread | NtQueueApcThread APC |

| 11 | FlsAlloc | Fiber Local Storage alloc |

| 12 | SetThreadContext | SetThreadContext resume |

| 13 | EtwSubscribe | ETW event subscription |

| 14 | EnumContextW | Callback on context |

| 15 | EnumDateFormatsW | Enum date formats |

| 16 | EnumDesktopsW | Enum desktops |

| 17 | EnumDisplayMonitors | Enum display monitors |

| 18 | EnumSystemLocalesEx | Enum system locales |

| 19 | EnumUILanguagesW | Enum UI languages |

| 20 | EnumWindowStationsW | Enum window stations |

| 21 | EnumWindows | Enum windows |

| 22 | EnumChildWindows | Enum child windows |

| 23 | MessageBoxIndirectW | MessageBoxIndirect callback |

| 24 | ChooseColorW | ChooseColor callback |

| 25 | EnumFontsW | Enum fonts |

| 26 | EnumFontFamiliesExW | Enum font families |

| 27 | CryptEnumOIDInfo | Crypt enum OID info |

| 28 | CertEnumSystemStore | Cert enum system store |

| 29 | CertEnumSystemStoreLocation | Cert store location |

| 30 | ImageGetDigestStream | Image get digest stream |

| 31 | SymEnumProcesses | Symbol enum processes |

  

## Examples

  

### Example 1: List Techniques

  

```bash

$  ShellVault.exe  --list

```

  

Output:

```
ID TECHNIQUE DESCRIPTION DLL

-- --------- ----------- ---

1 CreateThread Basic CreateThread execution kernel32.dll

2 CreateRemoteThread CreateRemoteThread injection kernel32.dll

3 QueueUserAPC APC QueueUserAPC injection kernel32.dll

...

```

  

### Example 2: Execute Shellcode

  

```bash

$  ShellVault.exe  payload.bin  3

```

  

Output:

```

[*] Executing via QueueUserAPC...

[+] Execution complete

```

  

### Example 3: Generate Loader Code

  

```bash

$  ShellVault.exe  payload.bin  --generate  loader.c

```

  

This generates a standalone C file you can compile independently.

  

## Requirements

  

- Windows 10/11 (x64)

- Visual Studio 2019 Build Tools (or later)

- MSVC C++ Compiler

  

## Building

  

### Using build.bat (Recommended)

  

```powershell

.\build.bat

```

  

### Manual Build

  

```powershell

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64

cl /EHsc /W4 ShellVault.cpp /Fe:ShellVault.exe  /link ws2_32.lib

```

  

## Output Binary

  

After building, the executable is: `ShellVault.exe`

  

## Branding

  

-  **Name**: ShellVault

-  **Author**: moscovium-mc

-  **Powered by**: VX-API

-  **Version**: 1.0.0

  

## License

  

See LICENSE file for details.

  

## Disclaimer

  

This tool is provided for educational and authorized security research purposes only. The author assumes no liability for any misuse or damage caused by this software.