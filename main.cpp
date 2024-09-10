#include <bitset>
#include <getopt.h>
#include <iomanip>
#include <iostream>
#include <memory>
#include <Windows.h>
#include "BitManipulation.h"
#include <cxxopts.hpp>
#include "CH341DLL_EN.H"


int proccedParse(int argc, const char *argv[]);

int main(int argc, const char *argv[]) {
    if (!proccedParse(argc, argv)) {
        return 1;
    }
    return 0;
}

int proccedParse(int argc, const char *argv[]) {
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
                ("speed",
                 "I2C interface speed/SCL frequency.\r\n"
                 "low=low speed/20KHz\r\n"
                 "standard=standard/100KHz (default value)\r\n"
                 "fast=fast/400KHz\r\n"
                 "high=high speed/750KHz",
                 cxxopts::value<std::string>()->default_value("standard"))
                ("wb", "Write byte for specified cell address with value.", cxxopts::value<std::vector<int> >())
                ("vrf", "Verified specified cell address.", cxxopts::value<bool>()->default_value("false"))
                ("rb", "Read byte from specified cell address.", cxxopts::value<int>());
        options.parse_positional({"i2c", "speed", "wb", "rb"});


        auto result = options.parse(argc, argv);

        if (result.count("help") || result.arguments().empty()) {
            std::cout << options.help() << std::endl;
            exit(0);
        }

        if (result.count("ver")) {
            CH341OpenDevice(0);
            ULONG ver;
            Sleep(20);
            ver = CH341GetDrvVersion();
            if (ver) {
                std::cout << "The driver version is " << ver << "." << std::endl;
            } else {
                std::cerr << "The driver is not installed." << std::endl;
            }
            Sleep(20);
            ver = CH341GetVersion();
            if (ver) {
                std::cout << "The DLL version is " << ver << "." << std::endl;
            } else {
                std::cerr << "The DLL is not installed." << std::endl;
            }
            Sleep(20);
            ver = CH341GetVerIC(0);
            if (ver) {
                std::cout << "The IC version is " << ver << "." << std::endl;
            } else {
                std::cerr << "The IC is not connected." << std::endl;
            }
            CH341CloseDevice(0);
        }

        if (result.count("i2c")) {
            std::cout << "Operate in I2C mode with " << std::hex << std::showbase << result["i2c"].as<int>() <<
                    " address of device." << std::endl;
            if (CH341OpenDevice(0) == INVALID_HANDLE_VALUE) {
                std::cerr << "Could not open device CH341." << std::endl;
                exit(1);
            } else {
                //Sleep(20);
                std::cout << "Device CH341 successfully opened." << std::endl;
                if (result.count("speed")) {
                    if (result["speed"].as<std::string>() == "low") {
                        CH341SetStream(0, I2C_SPEED_SCL_FREQUENCY_LOW);
                        std::cout << "I2C speed set as low." << std::endl;
                    } else if (result["speed"].as<std::string>() == "fast") {
                        CH341SetStream(0, I2C_SPEED_SCL_FREQUENCY_FAST);
                        std::cout << "I2C speed set as fast." << std::endl;
                    } else if (result["speed"].as<std::string>() == "high") {
                        CH341SetStream(0, I2C_SPEED_SCL_FREQUENCY_HIGH);
                        std::cout << "I2C speed set as high." << std::endl;
                    } else {
                        CH341SetStream(0, I2C_SPEED_SCL_FREQUENCY_STANDART);
                        std::cout << "I2C speed set as default." << std::endl;
                    }
                } else {
                    CH341SetStream(0, I2C_SPEED_SCL_FREQUENCY_STANDART);
                    std::cout << "I2C speed set as default." << std::endl;
                }
                if (result.count("wb")) {
                    UCHAR iByteTo = static_cast<UCHAR>(result["wb"].as<std::vector<int> >().at(1));
                    UCHAR iByteFrom = 0;
                    UCHAR iAddr = static_cast<UCHAR>(result["wb"].as<std::vector<int> >().at(0));
                    UCHAR iDevice = static_cast<UCHAR>(result["i2c"].as<int>());
                    std::cout << "Write byte for " << std::hex << std::showbase << static_cast<int>(iAddr) <<
                            " cell address with " << static_cast<int>(iByteTo) << " value." << std::endl;
                    Sleep(20);
                    if (CH341ReadI2C(0, iDevice, iAddr, (PUCHAR) &iByteFrom)) {
                        if (iByteTo == iByteFrom) {
                            std::cout << "The byte for " << std::hex << std::showbase << static_cast<int>(iAddr) <<
                                    " cell address with " << static_cast<int>(iByteTo) << " value already written." <<
                                    std::endl;
                        } else {
                            Sleep(20);
                            if (CH341WriteI2C(0, iDevice, iAddr, iByteTo)) {
                                std::cout << "Write byte for " << std::hex << std::showbase << static_cast<int>(iAddr)
                                        << " cell address with " << static_cast<int>(iByteTo) << " value successful." <<
                                        std::endl;

                                if (result.count("vrf")) {
                                    std::cout << "Verify byte for " << std::hex << std::showbase << static_cast<int>(
                                                iAddr) << " cell address with " << static_cast<int>(iByteTo) <<
                                            " value." << std::endl;
                                    Sleep(20);
                                    if (CH341ReadI2C(0, iDevice, iAddr, &iByteFrom)) {
                                        if (iByteTo == iByteFrom) {
                                            std::cout << "The byte for " << std::hex << std::showbase << static_cast<
                                                        int>(iAddr) <<
                                                    " cell address with " << static_cast<int>(iByteTo) <<
                                                    " value verified." << std::endl;
                                        } else {
                                            std::cerr << "The byte for " << std::hex << std::showbase << static_cast<
                                                int>(iAddr) << " cell address with " << static_cast<int>(
                                                iByteTo) << " value not verified." << std::endl;
                                        }
                                        //Sleep(20);
                                    } else {
                                        std::cerr << "Read byte for " << std::hex << std::showbase << static_cast<int>(
                                                    iAddr) <<
                                                " cell address unsuccessful." << std::endl;
                                    }
                                }
                            } else {
                                std::cerr << "Write byte for " << std::hex << std::showbase << static_cast<int>(iAddr)
                                        << " cell address with " << static_cast<int>(iByteTo) << " value unsuccessful."
                                        << std::endl;
                            }
                        }
                    }
                }
                if (result.count("rb")) {
                    UCHAR iByteFrom = 0;
                    UCHAR iAddr = static_cast<UCHAR>(result["rb"].as<int>());
                    UCHAR iDevice = static_cast<UCHAR>(result["i2c"].as<int>());
                    Sleep(20);
                    if (CH341ReadI2C(0, iDevice, iAddr, &iByteFrom)) {
                        std::cout << "The byte from " << std::hex << std::showbase << static_cast<
                                    int>(iAddr) << " cell address is " << static_cast<int>(iByteFrom) <<
                                " value." << std::endl;
                    } else {
                        std::cerr << "Read byte for " << std::hex << std::showbase << static_cast<int>(
                            iAddr) << " cell address unsuccessful." << std::endl;
                    }
                }
            }
            CH341CloseDevice(0);
            std::cout << "The device CH341 successfully closed." << std::endl;
            exit(0);
        }
    } catch (const cxxopts::exceptions::exception &e) {
        std::cerr << "Error parsing options: " << e.what() << std::endl;
        return false;
    }
    return 0;
}
