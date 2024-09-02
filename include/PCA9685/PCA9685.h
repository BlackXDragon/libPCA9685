/*

C++ Library to interface Raspberry Pi to
the 16-channel servo board controlled by
the PCA9685 IC through I2C protocol.

*/

// Multiple inclusion guard
#ifndef PCA9685_H
#define PCA9685_H

#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

#include <time.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

// Registers
#define SUBADR1			0x02
#define SUBADR2			0x03
#define SUBADR3			0x04
#define MODE1			0x00
#define PRESCALE		0xFE
#define LED0_ON_L		0x06
#define LED0_ON_H		0x07
#define LED0_OFF_L		0x08
#define LED0_OFF_H		0x09
#define ALLLED_ON_L		0xFA
#define ALLLED_ON_H		0xFB
#define ALLLED_OFF_L	0xFC
#define ALLLED_OFF_H	0xFD

// Functions

// Initialise and reset the PCA9685 IC
typedef struct {
	const char* i2c_device;
	int addr;
} init_args;
int f_PCA9685_init(init_args args);
#define PCA9685_init(...) f_PCA9685_init((init_args){__VA_ARGS__})

// Reset the PCA9685 IC
typedef struct {
	int fd;
	int addr;
} reset_args;
bool f_PCA9685_reset(reset_args args);
#define PCA9685_reset(...) f_PCA9685_reset((reset_args){__VA_ARGS__})

// Set the frequency of the PWM signal
typedef struct {
	int fd;
	float freq;
} setPWMFreq_args;
bool f_PCA9685_setPWMFreq(setPWMFreq_args args);
#define PCA9685_setPWMFreq(...) f_PCA9685_setPWMFreq((setPWMFreq_args){__VA_ARGS__})

// Set the PWM signal for a specific channel
typedef struct {
	int fd;
	int channel;
	int on;
	int off;
} setPWM_args;
bool f_PCA9685_setPWM(setPWM_args args);
#define PCA9685_setPWM(...) f_PCA9685_setPWM((setPWM_args){__VA_ARGS__})

// Set the PWM duty cycle for a specific channel in microseconds
typedef struct {
	int fd;
	int channel;
	int uS;
} setPWMuS_args;
bool f_PCA9685_setPWMuS(setPWMuS_args args);
#define PCA9685_setPWMuS(...) f_PCA9685_setPWMuS((setPWMuS_args){__VA_ARGS__})

// Close the file descriptor
bool PCA9685_close(int fd);

// Error code variable
int PCA9685_errno;

// String representation of the error code
const char* PCA9685_strerror(int err);

#endif
