#include <bitset>
#include <getopt.h>
#include <iomanip>
#include <iostream>
#include <memory>
#include <Windows.h>
#include "BitManipulation.h"
#include <cxxopts.hpp>
#include "CH341DLL.h"


int main(int argc, char *argv[]) {
    try {
        std::unique_ptr<cxxopts::Options> allocated(
            new cxxopts::Options("CH341CMD", " Command line utility for CH341 device."));
        auto &options = *allocated;
        options
                .positional_help("[optional args]")
                .show_positional_help();
        options.add_options("General")
                ("h, help", "Print help.")
                ("ver", "Get the driver, DLL, IC version number.");
        options.parse_positional({"help", "ver"});
        options.add_options("I2Cc")
                ("i2c", "I2C mode with argument as address of device.", cxxopts::value<int>())
                ("wb", "Write byte for specified cell address.", cxxopts::value<int>())
                ("rb", "Read byte from specified cell address.", cxxopts::value<int>());
        options.parse_positional({"i2c", "wb", "rb"});


        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            exit(0);
        }
        if (result.count("ver")) {
            ULONG ver;
            ver = CH341GetDrvVersion();
            if (ver) {
                std::cout << "The driver version is " << ver << "." << std::endl;
            } else {
                std::cerr << "The driver is not installed." << std::endl;
            }
            ver = CH341GetVersion();
            if (ver) {
                std::cout << "The DLL version is " << ver << "." << std::endl;
            } else {
                std::cerr << "The DLL is not installed." << std::endl;
            }
            ver = CH341GetVerIC(0);
            if (ver) {
                std::cout << "The IC version is " << ver << "." << std::endl;
            } else {
                std::cerr << "The IC is not connected." << std::endl;
            }
        }
        if (result.count("i2c")) {
            std::cout << "Operate in I2C mode with " << std::hex << std::showbase << result["i2c"].as<int>() <<
                    " address of device." << std::endl;
            if (result.count("wb")) {
                std::cout << "Write byte for " << std::hex << std::showbase << result["wb"].as<int>() <<
                        " cell address."
                        << std::endl;
            } else if (result.count("rb")) {
                if (CH341OpenDevice(0) == INVALID_HANDLE_VALUE) {
                    std::cerr << "Could not open device CH341." << std::endl;
                    return 1;
                } else {
                    std::cout << "Device CH341 successfully opened." << std::endl;
                    std::cout << "Read byte from " << std::hex << std::showbase << result["rb"].as<int>() <<
                            " cell address."
                            << std::endl;
                    char oByte;
                    if (CH341ReadI2C(0, result["i2c"].as<int>(), result["rb"].as<int>(), (PUCHAR) &oByte)) {
                        std::cout << "The value at address " << std::hex << std::showbase << result["rb"].as<int>() <<
                                " is 0b" << std::bitset<sizeof(oByte) * 8>(oByte) << "." << std::endl;
                    } else {
                        std::cout << "The value at address " << std::hex << std::showbase << result["rb"].as<int>() <<
                                " cannot be reading." << std::endl;
                    }
                }
                CH341CloseDevice(0);
                std::cout << "Device CH341 successfully closed." << std::endl;
            } else {
                std::cout << "Nothing to do. Specify a read or write operation" << std::endl;
            }
            exit(0);
        }
        if (result.count("wb")) {
            std::cout << "Write byte for specified cell address." << std::endl;
            exit(0);
        }
    } catch (const cxxopts::exceptions::exception &e) {
        std::cout << "error parsing options: " << e.what() << std::endl;
        return false;
    }


    /*
    std::cout << CH341GetDrvVersion() << std::endl;
    std::cout << CH341GetVersion() << std::endl;
    // Ініціалізація CH341
    if (CH341OpenDevice(0) == INVALID_HANDLE_VALUE) {
        std::cerr << "Не вдалося відкрити пристрій CH341." << std::endl;
        return 1;
    }
    std::cout << "Пристрій CH341 успішно відкрито." << std::endl;

    std::cout << CH341GetDeviceNameEx(0) << std::endl;

    ULONG Status = 0;
    PULONG iStatus = &Status;
    if (!CH341GetStatus(0, iStatus)) {
        std::cerr << "Не вдалося отримати статус CH341." << std::endl;
        return 1;
    }
    std::cout << "Статус " << std::bitset<sizeof(ULONG) * 8>(*iStatus) << std::endl;

    char oBuffer[64] = {0};
    ULONG ioLength = 64;

    CH341GetDeviceDescr(0, (PVOID) oBuffer, &ioLength);

    // Виведення у шістнадцятковому форматі
    for (size_t i = 0; i < ioLength; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (static_cast<int>(oBuffer[i]) & 0xFF) << ' ';
    }
    std::cout << std::endl;

    CH341GetConfigDescr(0, (PVOID) oBuffer, &ioLength);

    // Виведення у шістнадцятковому форматі
    for (size_t i = 0; i < ioLength; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (static_cast<int>(oBuffer[i]) & 0xFF) << ' ';
    }
    std::cout << std::endl;

    char oByte;
    for (int i = 0; i <= 0xff; ++i) {
        if (!CH341ReadI2C(0, 0x36, i, (PUCHAR) &oByte)) {
            std::cerr << "Не вдалося отримати byte at : " << i << std::endl;
            return 1;
        }
        std::cout << std::hex << std::setw(2) << std::setfill('0') << i << "\t" << std::bitset<
                    sizeof(char) * 8>(oByte)
                << std::endl;
    }


    CH341ReadI2C(0, 0x36, 0x09, (PUCHAR) &oByte);
    std::cout << "Читання конфігу " << std::bitset<sizeof(ULONG) * 8>(oByte) << std::endl;
    SET_PATTERN(oByte, 0b111, 0b000);
    CH341WriteI2C(0, 0x36, 0x09, oByte);
    std::cout << "МОдифікація конфігу " << std::bitset<sizeof(ULONG) * 8>(oByte) << std::endl;
    CH341ReadI2C(0, 0x36, 0x09, (PUCHAR) &oByte);
    std::cout << "Читання конфігу " << std::bitset<sizeof(ULONG) * 8>(oByte) << std::endl;

    //Sleep(10); // 10 мс
    CH341WriteI2C(0, 0x36, 0xff, 0x1);
    //Sleep(10); // 10 мс
    CH341WriteI2C(0, 0x36, 0xff, 0x11);
    //Sleep(10); // 10 мс
    CH341WriteI2C(0, 0x36, 0xff, 0x10);
    Sleep(20); // 10 мс
    CH341ReadI2C(0, 0x36, 0x09, (PUCHAR) &oByte);
    std::cout << "Читання конфігу OTP " << std::bitset<sizeof(ULONG) * 8>(oByte) << std::endl;


    // Закриття пристрою
    CH341CloseDevice(0);*/

    return 0;
}
