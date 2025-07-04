#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void printBytesAtOffset(const std::vector<unsigned char> &buffer, size_t offset, size_t length)
{
    std::cout << "[*] Bytes at offset 0x" << std::hex << offset << ": ";
    for (size_t i = 0; i < length && offset + i < buffer.size(); ++i)
    {
        printf("%02X ", buffer[offset + i]);
    }
    std::cout << std::endl;
}

size_t findPatternOffset(const std::vector<unsigned char> &buffer, const std::vector<unsigned char> &pattern)
{
    if (pattern.empty() || buffer.size() < pattern.size())
    {
        return std::string::npos;
    }

    std::cout << "[*] Searching for pattern: ";
    for (const auto &byte : pattern)
    {
        printf("%02X ", byte);
    }
    std::cout << std::endl;

    for (size_t i = 0; i <= buffer.size() - pattern.size(); ++i)
    {
        if (std::equal(pattern.begin(), pattern.end(), buffer.begin() + i))
        {
            return i;
        }
    }
    return std::string::npos;
}

bool patchVulkanBypass(const std::string &inputFile, const std::string &outputFile)
{
    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile)
    {
        std::cerr << "[!] Failed to open input file: " << inputFile << std::endl;
        return false;
    }

    // Pattern: E8 D9 86 F7 FF (CALL with specific displacement)
    std::vector<unsigned char> pattern = {0xE8, 0xD9, 0x86, 0xF7, 0xFF};
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(inFile), {});

    inFile.close();

    size_t matchOffset = std::string::npos;
    matchOffset = findPatternOffset(buffer, pattern);
    if (matchOffset == std::string::npos)
    {
        std::cerr << "[!] Failed to locate 'E8 D9 86 F7 FF' instruction in the binary." << std::endl;
        return false;
    }

    std::cout << "[*] Located CALL instruction at offset: 0x" << std::hex << matchOffset << std::endl;
    if (buffer[matchOffset] == 0xE8 &&
        buffer[matchOffset + 1] == 0xD9 &&
        buffer[matchOffset + 2] == 0x86 &&
        buffer[matchOffset + 3] == 0xF7 &&
        buffer[matchOffset + 4] == 0xFF)
    {

        // Patch: E8 D9 86 F7 FF → 31 C0 90 90 90 (XOR EAX,EAX; NOP; NOP; NOP)
        buffer[matchOffset] = 0x31;     // XOR
        buffer[matchOffset + 1] = 0xC0; // EAX,EAX
        buffer[matchOffset + 2] = 0x90; // NOP
        buffer[matchOffset + 3] = 0x90; // NOP
        buffer[matchOffset + 4] = 0x90; // NOP

        std::cout << "[+] Patched E8 D9 86 F7 FF -> 31 C0 90 90 90 (XOR EAX,EAX; NOP; NOP; NOP)" << std::endl;
        printBytesAtOffset(buffer, matchOffset, 5);
    }
    else
    {
        std::cerr << "[!] Expected CALL instruction bytes not found at matched offset." << std::endl;
        return false;
    }

    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile)
    {
        std::cerr << "[!] Failed to write patched file." << std::endl;
        return false;
    }

    outFile.write(reinterpret_cast<const char *>(buffer.data()), buffer.size());
    outFile.close();

    std::cout << "[*] Vulkan bypass patch applied successfully to: " << outputFile << std::endl;
    return true;
}

int main()
{
    std::cout << "============================================" << std::endl
              << "UnityPlayer Vulkan Bypass                   " << std::endl
              << "By CyR1en                                   " << std::endl
              << "============================================" << std::endl;

    const std::string dll = "UnityPlayer.dll";
    const std::string backup = "UnityPlayer_Original.dll";

    if (!fs::exists(dll))
    {
        std::cerr << "[!] " << dll << " not found in current directory." << std::endl;
        return 1;
    }

    try
    {
        if (!fs::exists(backup))
        {
            fs::rename(dll, backup);
            std::cout << "[+] Backed up original DLL as: " << backup << std::endl;
        }
        else
        {
            std::cerr << "[!] Backup already exists. Remove or rename " << backup << " before proceeding." << std::endl;
            return 1;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "[!] Backup failed: " << e.what() << std::endl;
        return 1;
    }

    if (!patchVulkanBypass(backup, dll))
    {
        std::cerr << "[x] Patching failed. Restoring original DLL..." << std::endl;
        fs::rename(backup, dll);
        return 1;
    }

    std::cout << std::endl;
    std::cout << "Press any key to exit..." << std::endl;
    std::cin.get();
    return 0;
}
