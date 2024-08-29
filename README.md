# PCA9685 C/C++ Library

This is a C/C++ library for the PCA9685 PWM controller. It is designed to work on any Linux system, including the Raspberry Pi.

## Installation

To install the library, clone the repository and run the following commands:

```bash
cd PCA9685
mkdir build && cd build
cmake ..
make
sudo make install
```

## Usage

To use the library, include the header file in your source code:

```c
#include <PCA9685/PCA9685.h>
```

Initialize the PCA9685 controller:

```c
int fd = PCA9685_init("/dev/i2c-1", 0x40);
```

The first parameter is the I2C device file and the second parameter (optional) is the I2C address of the PCA9685 controller. The I2C device defaults to "/dev/i2c-1" and the address defaults to 0x40.

Alternatively, a pre-initialised I2C file descriptor can be used:

```c
int fd = open("/dev/i2c-1", O_RDWR);
if (fd < 0) {
	// Error opening the I2C device
}

// Set the I2C slave address
if (ioctl(fd, I2C_SLAVE, addr) < 0) {
	// Error setting the I2C slave address
}

int fd = PCA9685_reset(fd, 0x40);
```

The PCA9685_init function also calls PCA9685_reset. It is not necessary to call PCA9685_reset if PCA9685_init is used.

The following function sets the PWM frequency of the PCA9685 controller (if frequency is not passed, it defaults to 50 Hz):

```c
PCA9685_setPWMFreq(fd, 50);
```

Now the PCA9685 controller is ready to use. You can set the PWM duty cycle for each channel:

```c
PCA9685_setPWMuS(fd, 0, 1500);
```

This sets the PWM duty cycle for channel 0 to 1500 microseconds. The second parameter is the channel number (0-15, default 0) and the third parameter is the pulse width in microseconds (default 1500).

The PCA9685_close function closes the I2C file descriptor:

```c
PCA9685_close(fd);
```

### Error Handling

The PCA9685_errorno variable contains the error code of the last function call. If the function call was successful, PCA9685_errorno is set to 0.

```c
if (PCA9685_errorno != 0) {
	// Error handling
}
```

The PCA9685_strerror function returns a string describing the error code:

```c
printf("Error: %s\n", PCA9685_strerror(PCA9685_errorno));
```

Table of error codes:

| Error Code | Description |
|------------|-------------|
| 0          | No error    |
| 1          | Failed to open I2C device |
| 2          | Unable to set I2C slave address |
| 3          | Failed to write to I2C device |
| 4          | Failed to read from I2C device |
| 5          | Invalid channel number |
| 6          | Invalid pulse width |
| 7          | Invalid frequency |
| 8		     | Invalid ON/OFF register values |
| 9          | Invalid I2C file descriptor |
| 10         | Invalid I2C address |
| 11         | Invalid I2C device file |

## License

This library is licensed under the MIT License. See the LICENSE file for more information.

## Contributing

Contributions are welcome! Please open an issue or submit a pull request.
