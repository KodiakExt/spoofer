#pragma once

#include <cstdint>
#include <iomanip>
#include <windows.h>
#include <iostream>
#include <bits/random.h>

class _UUID {
public:
    static std::string v4() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);

        std::stringstream ss;
        for (int i = 0; i < 16; ++i) {
            int randomValue = dis(gen);
            // Fix bits for version 4
            if (i == 6) {
                randomValue = (randomValue & 0x0F) | 0x30; // Set the version (bits 12-15) to 0100 (4)
            }
            // Fix bits for variant (RFC 4122)
            if (i == 8) {
                randomValue = (randomValue & 0x3F) | 0x80; // Set variant (bits 6-7) to 10
            }
            ss << std::setfill('0') << std::setw(2) << std::hex << randomValue;
            if (i == 3 || i == 5 || i == 7 || i == 9) {
                ss << '-';
            }
        }
        return ss.str();
    }
};
