# CH341CMD - Command Line Utility for CH341 Device

`CH341CMD` is a command-line utility for Windows designed for interfacing with the CH341 device, providing options for I2C communication and basic operations such as reading and writing bytes.
TODO Realize other modes.

## Usage

CH341CMD [OPTION...] [optional args]

General Options

    -h, --help
    Print help information about the available options.

    --ver
    Get the driver, DLL, and IC version number.

I2C Options

    --i2c [arg]
    Set I2C mode, where [arg] specifies the address of the device.

    --wb [arg]
    Write a byte to a specified cell address in the I2C device. [arg] should be the cell address.

    --rb [arg]
    Read a byte from a specified cell address in the I2C device. [arg] should be the cell address.

This project is licensed under the MIT License.

## Note

The driver [CH341SER](https://www.wch-ic.com/downloads/CH341SER_EXE.html) must be pre-installed.

## TODO
- [x] I2C byte
- [ ] I2C file
- [ ] SPI
