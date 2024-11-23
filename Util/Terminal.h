#pragma once

#include <windows.h>
#include <iostream>

class Terminal {
public:
    static bool isUserAdministrator() {
        bool isAdmin = false;
        HANDLE hToken = nullptr;

        if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
            TOKEN_ELEVATION elevation;
            DWORD dwSize;

            if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize)) {
                isAdmin = elevation.TokenIsElevated != 0;
            }

            CloseHandle(hToken);
        }

        return isAdmin;
    }

    static void setup() {
        if(!isUserAdministrator())
        {
            relaunchAsAdmin();
            exit(0);
        }
    }

    static void relaunchAsAdmin() {
        char exePath[MAX_PATH];
        GetModuleFileNameA(NULL, exePath, MAX_PATH);

        SHELLEXECUTEINFOA sei = { sizeof(sei) };
        sei.lpVerb = "runas";
        sei.lpFile = exePath;
        sei.nShow = SW_SHOWNORMAL;
        ShellExecuteExA(&sei);
    }
};
