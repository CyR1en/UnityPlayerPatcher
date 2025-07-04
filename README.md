# UnityPlayerPatcher - PEAK Vulkan Bypass Tool

A specialized binary patcher designed to disable the "force-vulkan" command-line argument functionality in Unity's rendering engine for the game PEAK. This tool prevents forced Vulkan activation by intercepting and neutralizing the graphics API validation process at the assembly level.

## Motivation

PEAK's current Steam launch configuration includes "force-vulkan" as its default launch option. As a result, launching the game directly (outside of Steam) will always apply the "force-vulkan" parameter. This forced Vulkan mode can cause compatibility issues, including preventing certain mods from working entirely. UnityPlayerPatcher addresses this issue by bypassing the forced Vulkan activation, allowing users to run the game and their mods without restriction.

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
   
4. **Run the Patcher**
    
    Double-click `UnityPlayerPatcher.exe` or execute the command in the same directory:
    
    ```cmd
    ./UnityPlayerPatcher.exe
    ```

5. **Expected Output**

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
    build\bin\release\UnityPlayerPatcher.exe
    ```

## Safety Considerations

- **Game Integrity**: The patch only affects Vulkan forcing; all other game functionality remains intact
- **Reversibility**: Always keeps original DLL backup for easy restoration
- **Anti-Virus**: Some AV software may flag binary patchers as suspicious (false positive)
- **Open Source**: The patcher is open source to verify that it's not doing anything malicious.
- **Automatically Built Executable**: The executable in the release section is built and uploaded through GitHub actions. The executable is not manually uploaded from my computer, thus making sure that it only contains functionalities that can be viewed publicly.

## License and Disclaimer

This project is licensed under the [MIT License](./LICENSE).

This tool is provided for educational and compatibility purposes. Users are responsible for compliance with applicable software licenses and terms of service. The author assumes no responsibility for any damage or issues resulting from the use of this patcher.

**Important**: This tool modifies game binaries. While designed to be safe and reversible, always maintain backups and use at your own discretion.
