#pragma once

#include <iostream>
#include <string>
#include <thread>

#include <string>
#include <span>
#include <cstring>
#include <fstream>
#include <thread>
#include <Windows.h>
#include <random>

class Logger {
public:
    inline static void info(const std::string& str, std::chrono::milliseconds sleep)
    {
        for (std::size_t i = 0; i < str.length(); ++i)
        {
            std::cout << str[i];
            std::cout.flush();
            std::this_thread::sleep_for(sleep);
        }
        std::cout << std::endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }

    inline static void notice(const std::string& str, std::chrono::milliseconds sleep)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        for (std::size_t i = 0; i < str.length(); ++i)
        {
            std::cout << str[i];
            std::cout.flush();
            std::this_thread::sleep_for(sleep);
        }
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }

    inline static void log_green(const std::string& str)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        std::cout << str << std::endl;
        std::cout.flush();
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }

    inline static void error(const std::string& str, std::chrono::milliseconds sleep)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        for (std::size_t i = 0; i < str.length(); ++i)
        {
            std::cout << str[i];
            std::cout.flush();
            std::this_thread::sleep_for(sleep);
        }
        std::cout << std::endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }

    inline static void warning(const std::string& str, std::chrono::milliseconds sleep)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        for (std::size_t i = 0; i < str.length(); ++i)
        {
            std::cout << str[i];
            std::cout.flush();
            std::this_thread::sleep_for(sleep);
        }
        std::cout << std::endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }

    static void displayProgressBar(int percentage) {
        const int totalBarLength = 35;
        int progress = (percentage * totalBarLength) / 100;

        char fullBlock = 219;
        char emptyBlock = 176;

        std::cout << "[";

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        for (int i = 0; i < progress; ++i) {
            std::cout << fullBlock;
        }

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
        for (int i = progress; i < totalBarLength; ++i) {
            std::cout << emptyBlock;
        }

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        std::cout << "] " << percentage << "%";
        std::cout << std::endl;
        std::cout.flush();
    }

    static void displayProgressBarInTo(int defaultPerc, int percentage) {
        for (int i = defaultPerc; i <= percentage; ++i)
        {
            clearConsole();
            log_green("(+) We are currently spoofing your data, please wait...");

            displayProgressBar(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    inline static void newline() {
        std::cout << std::endl;
    }
    inline static void clearConsole() {
        // It's Minecraft "Windows" 10 Edition buddy :D
        system("cls");
    }
};
