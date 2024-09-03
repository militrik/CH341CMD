#include <bitset>
#include <iomanip>
#include <iostream>
#include <Windows.h>
#include "BitManipulation.h"
#include "CH341DLL.h"

int main() {
    std::cout << CH341GetDrvVersion() << std::endl;
    std::cout << CH341GetVersion() << std::endl;
    // Ініціалізація CH341
    if (CH341OpenDevice(0) == INVALID_HANDLE_VALUE) {
        std::cerr << "Не вдалося відкрити пристрій CH341." << std::endl;
        return 1;
    }
    std::cout << "Пристрій CH341 успішно відкрито." << std::endl;

    ULONG Status = 0;
    PULONG iStatus = &Status;
    if (!CH341GetStatus(0, iStatus)) {
        std::cerr << "Не вдалося отримати статус CH341." << std::endl;
        return 1;
    }
    std::cout << "Статус " << std::bitset<sizeof(ULONG) * 8>(*iStatus) << std::endl;

    char oBuffer[64] ={0};
    ULONG ioLength = 64;

    CH341GetDeviceDescr(0, (PVOID)oBuffer, &ioLength);

    // Виведення у шістнадцятковому форматі
    for (size_t i = 0; i < ioLength; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (static_cast<int>(oBuffer[i]) & 0xFF) << ' ';
    }
    std::cout << std::endl;

    CH341GetConfigDescr(0, (PVOID)oBuffer, &ioLength);

    // Виведення у шістнадцятковому форматі
    for (size_t i = 0; i < ioLength; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (static_cast<int>(oBuffer[i]) & 0xFF) << ' ';
    }
    std::cout << std::endl;

    char oByte;
    for (int i = 0; i <= 0xff; ++i) {
        if (!CH341ReadI2C( 0, 0x36, i, (PUCHAR)&oByte)) {
            std::cerr << "Не вдалося отримати byte at : " << i<<  std::endl;
            return 1;
        }
        std::cout << std::hex << std::setw(2) << std::setfill('0') << i << "\t" << std::bitset<sizeof(char) * 8>(oByte)  << std::endl;
    }



    CH341ReadI2C( 0, 0x36, 0x09, (PUCHAR)&oByte);
    std::cout << "Читання конфігу " << std::bitset<sizeof(ULONG) * 8>(oByte) << std::endl;
    SET_PATTERN(oByte, 0b111, 0b000);
    CH341WriteI2C( 0, 0x36, 0x09, oByte);
    std::cout << "МОдифікація конфігу " << std::bitset<sizeof(ULONG) * 8>(oByte) << std::endl;
    CH341ReadI2C( 0, 0x36, 0x09, (PUCHAR)&oByte);
    std::cout << "Читання конфігу " << std::bitset<sizeof(ULONG) * 8>(oByte) << std::endl;

    //Sleep(10); // 10 мс
    CH341WriteI2C( 0, 0x36, 0xff, 0x1);
    //Sleep(10); // 10 мс
    CH341WriteI2C( 0, 0x36, 0xff, 0x11);
    //Sleep(10); // 10 мс
    CH341WriteI2C( 0, 0x36, 0xff, 0x10);
    Sleep(4); // 10 мс
    CH341ReadI2C( 0, 0x36, 0x09, (PUCHAR)&oByte);
    std::cout << "Читання конфігу OTP " << std::bitset<sizeof(ULONG) * 8>(oByte) << std::endl;


    // Закриття пристрою
    CH341CloseDevice(0);

    return 0;
}
