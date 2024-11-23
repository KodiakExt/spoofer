#pragma once

#include <algorithm>
#include <string>
#include <tlhelp32.h>
#include <psapi.h>
#include <sstream>
#include <span>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <Windows.h>
#include <random>
#include <memory>
#include <atomic>
#include <future>

struct Pattern {
    std::vector<BYTE> bytes;
    std::string mask;
};

class Game
{
public:
    static inline HMODULE module = nullptr;
    static inline HWND gameWindow;
    static inline DWORD proccessId;
    static inline HANDLE handle = nullptr;
    static inline uintptr_t baseAddress;
    static inline uintptr_t endAddress;

    static inline std::mutex resultsMutex;
    static inline std::atomic<bool> foundFlag;

    inline static boolean crashed()
    {
        return Game::GetProcessByName("Minecraft.Windows.exe") == nullptr;
    }

    inline static HANDLE GetProcessByName(const PCSTR name)
    {
        DWORD pid = 0;

        // Create toolhelp snapshot.
        const HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32 process;
        ZeroMemory(&process, sizeof(process));
        process.dwSize = sizeof(process);

        // Walkthrough all processes.
        if (Process32First(snapshot, &process))
        {
            do
            {
                // Compare process.szExeFile based on format of name, i.e., trim file path
                // trim .exe if necessary, etc.
                if (std::string(process.szExeFile) == std::string(name))
                {
                    pid = process.th32ProcessID;
                    proccessId = pid;
                    break;
                }
            } while (Process32Next(snapshot, &process));
        }

        CloseHandle(snapshot);

        if (pid != 0)
        {
            return OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
        }

        return nullptr;
    }

    inline static bool close() {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE) return false;

        PROCESSENTRY32 processEntry;
        processEntry.dwSize = sizeof(PROCESSENTRY32);
        bool processClosed = false;

        if (Process32First(hSnapshot, &processEntry)) {
            do {
                if (strcmp(processEntry.szExeFile, "Minecraft.Windows.exe") == 0) {
                    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processEntry.th32ProcessID);
                    if (hProcess) {
                        TerminateProcess(hProcess, 0);
                        CloseHandle(hProcess);
                        processClosed = true;
                    }
                }
            } while (Process32Next(hSnapshot, &processEntry));
        }

        CloseHandle(hSnapshot);
        return processClosed;
    }

    inline static HMODULE GetModule(const HANDLE pHandle)
    {
        DWORD cbNeeded;

        if (HMODULE hMods[1024]; EnumProcessModules(pHandle, hMods, sizeof(hMods), &cbNeeded)) {
            for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
            {
                TCHAR szModName[MAX_PATH];
                if (GetModuleFileNameEx(pHandle, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
                {
                    std::basic_string<TCHAR> wstrModName = szModName;
                    std::string wstrModContain = "Minecraft.Windows.exe";
                    TCHAR szModContain[22];
                    ua_tcscpy(szModContain, wstrModContain.c_str());
                    if (wstrModName.find(std::basic_string<TCHAR>(szModContain)) != std::string::npos)
                    {
                        //CloseHandle(pHandle);
                        return hMods[i];
                    }
                }
            }
        }
        return nullptr;
    }

    inline static std::string GetMcVersion() {
        try {
            std::string version = "Unknown";
            HANDLE pipeRead, pipeWrite;
            SECURITY_ATTRIBUTES saAttr;

            saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
            saAttr.bInheritHandle = TRUE;
            saAttr.lpSecurityDescriptor = NULL;

            if (!CreatePipe(&pipeRead, &pipeWrite, &saAttr, 0)) {
                std::cerr << "Failed to create pipe: " << GetLastError() << std::endl;
                return version;
            }

            STARTUPINFO si;
            PROCESS_INFORMATION pi;
            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            si.hStdOutput = pipeWrite;
            si.dwFlags |= STARTF_USESTDHANDLES;

            ZeroMemory(&pi, sizeof(pi));

            std::string command = "powershell.exe -Command \"Get-AppPackage -name Microsoft.MinecraftUWP | select -expandproperty Version\"";
            if (!CreateProcess(NULL, LPSTR(command.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
                std::cerr << "Failed to create process: " << GetLastError() << std::endl;
                CloseHandle(pipeRead);
                CloseHandle(pipeWrite);
                return version;
            }

            CloseHandle(pipeWrite);

            char buffer[4096];
            DWORD bytesRead;
            std::string output;
            while (ReadFile(pipeRead, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead != 0) {
                output.append(buffer, bytesRead);
            }

            CloseHandle(pipeRead);
            version = output;

            return version;
        }
        catch (const std::exception& ex) {
            std::cerr << "Unable to get version: " << ex.what() << std::endl;
            return "Unknown";
        }
    }

    static std::string roundVersion(const std::string& version) {
        if (version.length() < 7) {
            return version;
        }

        return version.substr(0, 7);
    }

    static void patchBytes(void* dst, void* src, unsigned int size) {
        DWORD oldprotect;
        VirtualProtectEx(handle, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
        WriteProcessMemory(handle, dst, src, size, 0);
        VirtualProtectEx(handle, dst, size, oldprotect, &oldprotect);
    }

    static std::string getUserName() {
        char* userName = getenv("USERNAME");
        return userName ? std::string(userName) : "";
    }

    static bool endsWith(const std::string& str, const std::string& suffix) {
        if (str.length() >= suffix.length()) {
            return (0 == str.compare(str.length() - suffix.length(), suffix.length(), suffix));
        }
        return false;
    }

    inline static void removeSpecificFiles(const std::string& directory, const std::vector<std::string>& extensions) {
        std::string basePath = "C:\\Users\\" + getUserName() + "\\AppData\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe";
        std::string fullPath = basePath + (directory.front() == '\\' ? directory : "\\" + directory);

        DWORD attributes = GetFileAttributesA(fullPath.c_str());
        if (attributes == INVALID_FILE_ATTRIBUTES) {
            return;
        }

        if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
            WIN32_FIND_DATAA findData;
            HANDLE hFind = FindFirstFileA((fullPath + "\\*").c_str(), &findData);
            if (hFind != INVALID_HANDLE_VALUE) {
                do {
                    const std::string item = findData.cFileName;
                    if (item != "." && item != "..") {
                        const std::string itemPath = fullPath + "\\" + item;

                        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                            removeSpecificFiles(itemPath, extensions);
                        } else {
                            for (const auto& ext : extensions) {
                                if (endsWith(item, ext)) {
                                    DeleteFileA(itemPath.c_str());
                                    break;
                                }
                            }
                        }
                    }
                } while (FindNextFileA(hFind, &findData));
                FindClose(hFind);
            }
        }
    }

    static void remove(const std::string& suffix) {
        std::string basePath = "C:\\Users\\" + getUserName() + "\\AppData\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe";
        std::string fullPath = basePath + (suffix.front() == '\\' ? suffix : "\\" + suffix);

        WIN32_FIND_DATAA findData;
        HANDLE hFind;

        DWORD attributes = GetFileAttributesA(fullPath.c_str());
        if (attributes == INVALID_FILE_ATTRIBUTES) {
            return;
        }

        if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
            hFind = FindFirstFileA((fullPath + "\\*").c_str(), &findData);
            if (hFind != INVALID_HANDLE_VALUE) {
                do {
                    const std::string item = findData.cFileName;
                    if (item != "." && item != "..") {
                        const std::string itemPath = fullPath + "\\" + item;
                        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                            remove(itemPath);
                        } else DeleteFileA(itemPath.c_str());
                    }
                } while (FindNextFileA(hFind, &findData));
                FindClose(hFind);
            }

            RemoveDirectoryA(fullPath.c_str());
        } else DeleteFileA(fullPath.c_str());
    }

    #include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <Windows.h>

static std::vector<void*> scanString(HANDLE handle, const std::string& searchString) {
        std::vector<void*> foundAddresses;
        MEMORY_BASIC_INFORMATION memInfo;
        void* currentAddress = nullptr;
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;
        std::mutex addressesMutex;
        std::atomic<int> foundCount(0);

        auto scanTask = [&](void* baseAddr, SIZE_T regionSize) {
            std::vector<char> pageData(regionSize);
            SIZE_T bytesRead;
            if (ReadProcessMemory(handle, baseAddr, pageData.data(), regionSize, &bytesRead)) {
                std::string pageString(pageData.data(), bytesRead);
                size_t pos = 0;
                while ((pos = pageString.find(searchString, pos)) != std::string::npos) {
                    if (foundCount >= 15) return;

                    std::lock_guard lock(addressesMutex);
                    foundAddresses.push_back(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(baseAddr) + pos));
                    ++foundCount;
                    pos += searchString.length();
                }
            }
        };

        while (VirtualQueryEx(handle, currentAddress, &memInfo, sizeof(memInfo)) != 0) {
            if (memInfo.State == MEM_COMMIT && memInfo.Protect != PAGE_NOACCESS) {
                if (threads.size() >= numThreads) {
                    for (auto& t : threads) {
                        if (t.joinable()) {
                            t.join();
                        }
                    }
                    threads.clear();
                }

                threads.emplace_back(scanTask, currentAddress, memInfo.RegionSize);
            }
            currentAddress = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(currentAddress) + memInfo.RegionSize);

            if (foundCount >= 15) {
                break;
            }
        }

        for (auto& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }

        return foundAddresses;
    }

    static std::string getDeviceId()
    {
        auto getLocalAppDataPath = []() -> std::string {
            const char* buf = getenv("LOCALAPPDATA");
            return buf != nullptr ? buf : "";
        };

        if (const std::string localAppDataPath = getLocalAppDataPath(); !localAppDataPath.empty())
        {
            const std::string hsFilePath = localAppDataPath + R"(\Packages\Microsoft.MinecraftUWP_8wekyb3d8bbwe\LocalState\games\com.mojang\minecraftpe\hs)";

            if (std::ifstream hsFile(hsFilePath); hsFile.is_open())
            {
                std::string line;
                size_t lineCount = 0;
                while (getline(hsFile, line))
                {
                    if (lineCount == 1)
                    {
                        return line;
                    }
                    ++lineCount;
                }

                hsFile.close();
            }
        }
        return "";
    }
};
