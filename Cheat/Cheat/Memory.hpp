#pragma once
#include <sstream>
#include "Brand.hpp"
REVERS_STAMP(Memory_hpp)

template <class t>
t ReadMemory(uintptr_t address) {
    t read;
    ReadProcessMemory(Game.hProcess, (LPVOID)address, &read, sizeof(t), NULL);
    return read;
}

template <class T>
void WriteMemory(uintptr_t address, T value) {
    WriteProcessMemory(Game.hProcess, (LPVOID)address, &value, sizeof(T), NULL);
}

uintptr_t GetBaseAddress(const std::string& moduleName) {
    if (Game.pID == 0) {
        return 0;
    }

    HANDLE moduleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, Game.pID);
    if (moduleSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    MODULEENTRY32 moduleEntry;
    moduleEntry.dwSize = sizeof(moduleEntry);

    if (!Module32First(moduleSnapshot, &moduleEntry)) {
        CloseHandle(moduleSnapshot);
        return 0;
    }

    do {
        std::string currentModuleName(moduleEntry.szModule);

        if (currentModuleName.compare(moduleName) == 0) {
            uintptr_t baseAddress = (uintptr_t)moduleEntry.modBaseAddr;
            CloseHandle(moduleSnapshot);
            return baseAddress;
        }
    } while (Module32Next(moduleSnapshot, &moduleEntry));

    CloseHandle(moduleSnapshot);
    return 0;
}

typedef NTSTATUS(NTAPI* tNtWriteVirtualMemory)(HANDLE, PVOID, PVOID, ULONG, PULONG);
tNtWriteVirtualMemory oNtWriteVirtualMemory = nullptr;

void WriteBytesNt(uintptr_t address, uint8_t* patch, size_t size) {
    ULONG bytesWritten = 0;
    if (!oNtWriteVirtualMemory) {
        HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
        if (hNtdll) {
            oNtWriteVirtualMemory = (tNtWriteVirtualMemory)GetProcAddress(hNtdll, "NtWriteVirtualMemory");
        }
    }

    if (oNtWriteVirtualMemory) {
        oNtWriteVirtualMemory(Game.hProcess, reinterpret_cast<PVOID>(address), patch, static_cast<ULONG>(size), &bytesWritten);
    }
}

void WriteBytes(uintptr_t address, uint8_t* patch, size_t size) {
    if (!Game.hProcess || !address || !patch || size == 0) return;

    if (!oNtWriteVirtualMemory) {
        HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
        if (hNtdll) {
            oNtWriteVirtualMemory = (tNtWriteVirtualMemory)GetProcAddress(hNtdll, "NtWriteVirtualMemory");
        }
    }

    DWORD oldProtect;
    if (VirtualProtectEx(Game.hProcess, (LPVOID)address, size, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        if (oNtWriteVirtualMemory) {
            ULONG bytesWritten = 0;
            oNtWriteVirtualMemory(Game.hProcess, reinterpret_cast<PVOID>(address), patch, static_cast<ULONG>(size), &bytesWritten);
        }
        else {
            SIZE_T bytesWritten = 0;
            WriteProcessMemory(Game.hProcess, (LPVOID)address, patch, size, &bytesWritten);
        }
        VirtualProtectEx(Game.hProcess, (LPVOID)address, size, oldProtect, &oldProtect);
    }
}

std::string GetProcessNameByPID(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (hProcess == nullptr) {
        return "";
    }

    char processName[MAX_PATH];
    DWORD dwSize = MAX_PATH;
    if (QueryFullProcessImageNameA(hProcess, 0, processName, &dwSize)) {
        std::string fullPath(processName);
        size_t pos = fullPath.find_last_of("\\/");
        if (pos != std::string::npos) {
            fullPath = fullPath.substr(pos + 1);
        }
        CloseHandle(hProcess);
        return fullPath;
    }
    CloseHandle(hProcess);
    return "";
}

uintptr_t GetBaseAddress() {
    if (Game.pID == 0) {
        return 0;
    }

    HANDLE moduleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, Game.pID);
    if (moduleSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    MODULEENTRY32 moduleEntry;
    moduleEntry.dwSize = sizeof(moduleEntry);
    if (!Module32First(moduleSnapshot, &moduleEntry)) {
        CloseHandle(moduleSnapshot);
        return 0;
    }

    do {

        std::string currentModuleName(moduleEntry.szModule);
        if (currentModuleName.compare(GetProcessNameByPID(Game.pID)) == 0) {
            uintptr_t baseAddress = (uintptr_t)moduleEntry.modBaseAddr;
            CloseHandle(moduleSnapshot);
            return baseAddress;
        }
    } while (Module32Next(moduleSnapshot, &moduleEntry));
    CloseHandle(moduleSnapshot);
    return 0;
}

#include <Windows.h>
#include <winternl.h>
#include <string>

typedef NTSTATUS(NTAPI* tNtReadVirtualMemory)(HANDLE, LPCVOID, PVOID, SIZE_T, PSIZE_T);

inline tNtReadVirtualMemory GetNtReadVirtualMemory()
{
    static tNtReadVirtualMemory fn = nullptr;
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
        if (hNtdll) {
            fn = (tNtReadVirtualMemory)GetProcAddress(hNtdll, "NtReadVirtualMemory");
            if (!fn)
                fn = (tNtReadVirtualMemory)GetProcAddress(hNtdll, "ZwReadVirtualMemory");
        }
    }
    return fn;
}

inline bool ReadProcessBytes(uintptr_t address, void* buffer, size_t size)
{
    if (!Game.hProcess || !address || !buffer || size == 0)
        return false;

    if (auto readFn = GetNtReadVirtualMemory()) {
        SIZE_T bytesRead = 0;
        return readFn(Game.hProcess, (LPCVOID)address, buffer, size, &bytesRead) >= 0 && bytesRead == size;
    }

    SIZE_T bytesRead = 0;
    return ReadProcessMemory(Game.hProcess, (LPCVOID)address, buffer, size, &bytesRead) != 0 && bytesRead == size;
}

std::string ReadString(uintptr_t Addr) {
    const int bufferSize = 256;
    char buffer[bufferSize];
    int bytesRead = 0;
    bool success = true;

    while (bytesRead < bufferSize) {
        char character;
        if (!ReadProcessBytes(Addr + bytesRead, &character, sizeof(char)))
            break;
        buffer[bytesRead] = character;
        if (character == '\0') { break; }
        bytesRead++;
    }

    if (bytesRead == bufferSize) {
        success = false;
    }

    if (!success) {
        return "";
    }
    return std::string(buffer);
}

inline std::string ReadStringW(uintptr_t Addr) {
    if (!Addr || Addr < 0x1000) {
        return "";
    }

    const int bufferSize = 256;
    wchar_t buffer[bufferSize] = { 0 };
    int charsRead = 0;
    SIZE_T bytesRead = 0;

    while (charsRead < bufferSize - 1) {
        wchar_t character = 0;
        SIZE_T bytesReadThisTime = 0;

        if (!ReadProcessMemory(Game.hProcess, (LPCVOID)(Addr + charsRead * sizeof(wchar_t)),
            &character, sizeof(wchar_t), &bytesReadThisTime)) {
            break;
        }

        if (bytesReadThisTime != sizeof(wchar_t)) {
            break;
        }

        buffer[charsRead] = character;
        if (character == L'\0') {
            break;
        }
        charsRead++;
    }

    if (charsRead == 0) {
        return "";
    }

    int utf8Size = WideCharToMultiByte(CP_UTF8, 0, buffer, charsRead, NULL, 0, NULL, NULL);
    if (utf8Size <= 0) {
        return "";
    }

    std::string result(utf8Size, 0);
    int converted = WideCharToMultiByte(CP_UTF8, 0, buffer, charsRead, &result[0], utf8Size, NULL, NULL);
    if (converted <= 0) {
        return "";
    }

    if (converted > 0 && result[converted - 1] == '\0') {
        result.resize(converted - 1);
    }
    else {
        result.resize(converted);
    }

    return result;
}

inline std::string ReadStringAuto(uintptr_t Addr) {
    unsigned char firstBytes[8]{};
    ReadProcessBytes(Addr, firstBytes, sizeof(firstBytes));

    bool tryUTF16 = false;

    if ((firstBytes[0] == 0xFF && firstBytes[1] == 0xFE) || (firstBytes[0] == 0xFE && firstBytes[1] == 0xFF)) {
        tryUTF16 = true;
    }
    else if (firstBytes[1] == 0 && firstBytes[0] != 0 && firstBytes[0] < 0x80 &&
        firstBytes[3] == 0 && firstBytes[2] != 0 && firstBytes[2] < 0x80) {
        tryUTF16 = true;
    }
    else if ((firstBytes[0] >= 0xD8 && firstBytes[0] <= 0xDF) ||
        (firstBytes[1] >= 0xD8 && firstBytes[1] <= 0xDF)) {
        tryUTF16 = true;
    }
    else {
        int zeroCount = 0;
        int nonZeroCount = 0;
        for (int i = 0; i < 8; i += 2) {
            if (firstBytes[i] == 0 && firstBytes[i + 1] != 0) zeroCount++;
            if (firstBytes[i] != 0 && firstBytes[i + 1] == 0) nonZeroCount++;
        }
        if (zeroCount >= 2 || nonZeroCount >= 2) {
            tryUTF16 = true;
        }
    }

    if (tryUTF16) {
        std::string utf8Result = ReadStringW(Addr);
        if (!utf8Result.empty()) {
            int validCharCount = 0;
            int totalChars = 0;
            for (size_t i = 0; i < utf8Result.length() && i < 100; ++i) {
                totalChars++;
                unsigned char c = static_cast<unsigned char>(utf8Result[i]);
                if ((c >= 32 && c <= 126) || (c >= 0xC0 && c <= 0xF7)) {
                    validCharCount++;
                }
            }
            if (validCharCount > 0 && (totalChars == 0 || validCharCount * 2 > totalChars)) {
                return utf8Result;
            }
        }
    }

    std::string ansiResult = ReadString(Addr);
    if (!ansiResult.empty()) {
        int validCharCount = 0;
        for (size_t i = 0; i < ansiResult.length() && i < 100; ++i) {
            unsigned char c = static_cast<unsigned char>(ansiResult[i]);
            if ((c >= 32 && c <= 126) || (c >= 0xC0 && c <= 0xF7)) {
                validCharCount++;
            }
        }
        if (validCharCount > 0) {
            return ansiResult;
        }
    }

    if (tryUTF16) {
        std::string utf8Result = ReadStringW(Addr);
        if (!utf8Result.empty()) {
            return utf8Result;
        }
    }

    return ansiResult;
}

class MemoryScanner {
public:
    static uintptr_t FindSignature(const std::vector<uint8_t>& pattern) {
        if (!Game.hProcess || !Offsets.GameBase) return 0;

        MEMORY_BASIC_INFORMATION mbi;
        uintptr_t address = Offsets.GameBase;
        uintptr_t maxAddress = address + 0x7FFFFFFF;

        while (address < maxAddress) {
            if (VirtualQueryEx(Game.hProcess, (LPCVOID)address, &mbi, sizeof(mbi)) == 0) {
                address += 0x1000;
                continue;
            }

            if (mbi.State == MEM_COMMIT && (mbi.Protect & PAGE_EXECUTE_READWRITE || mbi.Protect & PAGE_EXECUTE_READ)) {
                std::vector<uint8_t> buffer(mbi.RegionSize);
                SIZE_T bytesRead;

                if (ReadProcessMemory(Game.hProcess, mbi.BaseAddress, buffer.data(), mbi.RegionSize, &bytesRead)) {
                    for (size_t i = 0; i <= bytesRead - pattern.size(); ++i) {
                        bool found = true;
                        for (size_t j = 0; j < pattern.size(); ++j) {
                            if (pattern[j] != 0x00 && buffer[i + j] != pattern[j]) {
                                found = false;
                                break;
                            }
                        }
                        if (found) {
                            return (uintptr_t)mbi.BaseAddress + i;
                        }
                    }
                }
            }

            address = (uintptr_t)mbi.BaseAddress + mbi.RegionSize;
        }

        return 0;
    }

    static uintptr_t FindSignature(const std::vector<uint8_t>& pattern, uintptr_t startAddress, size_t searchSize) {
        if (!Game.hProcess) return 0;

        std::vector<uint8_t> buffer(searchSize);
        SIZE_T bytesRead;

        if (ReadProcessMemory(Game.hProcess, (LPCVOID)startAddress, buffer.data(), searchSize, &bytesRead)) {
            for (size_t i = 0; i <= bytesRead - pattern.size(); ++i) {
                bool found = true;
                for (size_t j = 0; j < pattern.size(); ++j) {
                    if (pattern[j] != 0x00 && buffer[i + j] != pattern[j]) {
                        found = false;
                        break;
                    }
                }
                if (found) {
                    return startAddress + i;
                }
            }
        }

        return 0;
    }
};

inline std::vector<uint8_t> Pattern2Vector(const std::string& Pattern) {
    std::vector<uint8_t> signature;
    std::istringstream stream(Pattern);
    std::string byteStr;

    while (stream >> byteStr) {
        if (byteStr == "?" || byteStr == "??") {
            signature.push_back(0x00);
        }
        else {
            try {
                signature.push_back(static_cast<uint8_t>(std::stoul(byteStr, nullptr, 16)));
            }
            catch (...) {
                signature.push_back(0x00);
            }
        }
    }

    return signature;
}

inline uintptr_t FindSignature(const std::vector<uint8_t>& pattern, uintptr_t startAddress, size_t searchSize) {
    if (!Game.hProcess || pattern.empty()) return 0;

    const size_t chunkSize = 0x100000;
    std::vector<uint8_t> buffer(chunkSize);

    for (size_t offset = 0; offset < searchSize; offset += chunkSize - pattern.size()) {
        SIZE_T bytesRead = 0;
        size_t currentChunkSize = (chunkSize < searchSize - offset) ? chunkSize : (searchSize - offset);

        if (!ReadProcessMemory(Game.hProcess, (LPCVOID)(startAddress + offset), buffer.data(), currentChunkSize, &bytesRead)) {
            continue;
        }

        if (bytesRead < pattern.size()) continue;

        for (size_t i = 0; i <= bytesRead - pattern.size(); ++i) {
            bool found = true;
            for (size_t j = 0; j < pattern.size(); ++j) {
                if (pattern[j] != 0x00 && buffer[i + j] != pattern[j]) {
                    found = false;
                    break;
                }
            }
            if (found) {
                return startAddress + offset + i;
            }
        }
    }

    return 0;
}

inline uintptr_t FindSignatureStr(const std::string& Pattern, uintptr_t ModuleBase, uintptr_t ModuleBaseSize) {
    std::vector<uint8_t> signature = Pattern2Vector(Pattern);
    if (ModuleBase != 0 && ModuleBaseSize != 0) {
        return FindSignature(signature, ModuleBase, ModuleBaseSize);
    }
    else {
        return 0;
    }
}