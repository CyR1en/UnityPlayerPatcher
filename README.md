# UnityPlayerPatcher - PEAK Vulkan Bypass Tool

A specialized binary patcher designed to disable the "force-vulkan" command-line argument functionality in Unity's rendering engine for the game PEAK. This tool prevents forced Vulkan activation by intercepting and neutralizing the graphics API validation process at the assembly level.

## Overview

UnityPlayerPatcher modifies the UnityPlayer.dll binary to bypass Vulkan validation, ensuring that the "force-vulkan" command-line argument is effectively ignored. This is particularly useful for users experiencing compatibility issues, performance problems, or stability concerns when Vulkan is forcibly enabled through launch parameters.

## How to Use
1. **Download the Patcher**

   Link: [https://github.com/CyR1en/UnityPlayerPatcher/releases/download/0.1.0/UnityPlayerPatcher.exe](https://github.com/CyR1en/UnityPlayerPatcher/releases/download/0.1.0/UnityPlayerPatcher.exe)

2. **Locate Your Game Directory**

    Navigate to your PEAK installation folder where `UnityPlayer.dll` is located. This is typically:
    
    ```
    [Steam Directory]\steamapps\common\PEAK\
    ```

3. **Place the Patcher**

    Copy `UnityPlayerPatcher.exe` into the same directory as `UnityPlayer.dll`.
    3. **Run the Patcher**
    
    Execute the patcher from command line or by double-clicking:
    
    ```cmd
    UnityPlayerPatcher.exe
    ```

4. **Expected Output**

    ```
    ============================================
    UnityPlayer Vulkan Validation Bypass        
    By CyR1en                                   
    ============================================
    [+] Backed up original DLL as: UnityPlayer_Original.dll
    [*] Located CALL instruction at offset: 0x[address]
    [*] Bytes at offset 0x[address]: E8 D9 86 F7 FF
    [+] Patched E8 D9 86 F7 FF -> 31 C0 90 90 90 (XOR EAX,EAX; NOP; NOP; NOP)
    [*] Bytes at offset 0x[address]: 31 C0 90 90 90
    [*] Vulkan validation bypass patch applied successfully to: UnityPlayer.dll
    ```

### Restoring Original Functionality

To restore the original Vulkan forcing capability:

1. Delete the patched `UnityPlayer.dll`
2. Rename `UnityPlayer_Original.dll` back to `UnityPlayer.dll`

## How It Works

### Technical Implementation

The patcher employs binary-level modification to intercept Unity's graphics API selection process during the handling of the "force-vulkan" command-line argument. Here's the technical breakdown:

#### Target Pattern Identification

The tool searches for the specific byte sequence `E8 D9 86 F7 FF`, which represents a `CALL FUN_1806406b0` instruction in the UnityPlayer.dll binary. Based on the surrounding code context and execution flow, this function call appears to be part of the Vulkan activation pathway, though its exact purpose is inferred from the assembly analysis.

#### Assembly-Level Modification

```assembly
; Original instruction
E8 D9 86 F7 FF    CALL FUN_1806406b0

; Patched replacement  
31 C0             XOR EAX,EAX     ; Set return value to 0 (failure)
90                NOP             ; No operation
90                NOP             ; No operation  
90                NOP             ; No operation
```


#### Execution Flow Modification

1. **Before Patch**: Command-line parser detects "force-vulkan" → calls unknown function → if successful, presumably sets Vulkan-related flags
2. **After Patch**: Command-line parser detects "force-vulkan" → function call bypassed with failure return → Vulkan forcing pathway interrupted

## Compilation

### Requirements

- **CMake** 3.10 or higher
- **C++17 compatible compiler** (Visual Studio 2017+, GCC 7+, Clang 5+)
- **Windows SDK** (for filesystem operations)


### Build Instructions

1. **Clone or Download Source**

    Ensure you have the project structure:
    
    ```
    UnityPlayerPatcher/
    ├── CMakeLists.txt
    ├── src/
    │   └── main.cpp
    └── README.md
    ```

2. **Compile the Project**

    ```cmd
    cmake --build build --config Release
    ```

3. **Locate Executable**

    The compiled executable will be available at:
    
    ```
    build/bin/UnityPlayerPatcher.exe
    ```

### Build Options

For different build configurations:

```cmd
# Debug build (with debug symbols)
cmake --build build --config Debug

# Release build (optimized)
cmake --build build --config Release

# Specify generator (if needed)
cmake -G "Visual Studio 16 2019" ..
```


## Safety Considerations

- **Game Integrity**: The patch only affects Vulkan forcing; all other game functionality remains intact
- **Reversibility**: Always keeps original DLL backup for easy restoration
- **Version Compatibility**: Designed specifically for PEAK's Unity version
- **Anti-Virus**: Some AV software may flag binary patchers as suspicious (false positive)

## License and Disclaimer

This project is licensed under the [MIT License](./LICENSE).

This tool is provided for educational and compatibility purposes. Users are responsible for compliance with applicable software licenses and terms of service. The author assumes no responsibility for any damage or issues resulting from the use of this patcher.

**Important**: This tool modifies game binaries. While designed to be safe and reversible, always maintain backups and use at your own discretion.
