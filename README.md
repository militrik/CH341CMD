# CH341CMD - Command Line Utility for CH341 Device

`CH341CMD` is a command-line utility for Windows designed for interfacing with the CH341 device, providing options for I2C communication and basic operations such as reading and writing bytes.
TODO Realize other modes.

## Usage

CH341CMD [OPTION...] [optional args]

### General Options:
- `-h`, `--help`    Print help.
- `--ver`    Get the driver, DLL, IC version number.

### I2C Options:
- `--i2c <device>`    I2C mode with argument as the address of the device.
- `--speed <speed>`    I2C interface speed / SCL frequency.
    - `low` = low speed / 20KHz
    - `standard` = standard / 100KHz (default)
    - `fast` = fast / 400KHz
    - `high` = high speed / 750KHz
- `--wb <address,value>`    Write byte for the specified cell address with comma-separated value.
- `--wf <path>`    Write file in Intel HEX or binary format.
- `--vrf`    Verify the write procedure.
- `--rb <address>`    Read byte from the specified cell address.

This project is licensed under the MIT License.

## Note

The driver [CH341SER](https://www.wch-ic.com/downloads/CH341SER_EXE.html) must be pre-installed.

## TODO
- [x] I2C byte
- [ ] I2C file
- [ ] SPI
