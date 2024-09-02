#include <PCA9685/PCA9685.h>

int f_PCA9685_init(init_args args) {
	char* i2c_device = args.i2c_device? args.i2c_device : "/dev/i2c-1";
	int addr = args.addr? args.addr : 0x40;

	if (access(i2c_device, F_OK) < 0) {
		PCA9685_errno = 11;
		return -1;
	}

	if (addr < 0x00 || addr > 0x7F) {
		PCA9685_errno = 10;
		return -1;
	}

	int fd = open(i2c_device, O_RDWR);
	if (fd < 0) {
		PCA9685_errno = 1;
		return -1;
	}

	if (ioctl(fd, I2C_SLAVE, addr) < 0) {
		PCA9685_errno = 2;

		// Close the file descriptor
		close(fd);

		return -1;
	}

	// Reset the PCA9685 IC
	PCA9685_reset(fd, addr);

	return fd;
}

bool f_PCA9685_reset(reset_args args) {
	int fd = args.fd? args.fd : -1;
	int addr = args.addr? args.addr : 0x40;

	if (fd < 0) {
		PCA9685_errno = 9;
		return false;
	}

	if (addr < 0x00 || addr > 0x7F) {
		PCA9685_errno = 10;
		return false;
	}

	if (ioctl(fd, I2C_SLAVE, addr) < 0) {
		PCA9685_errno = 2;
		return false;
	}

	if (i2c_smbus_write_byte_data(fd, MODE1, 0x00) < 0) {
		PCA9685_errno = 3;
		return false;
	}

	return true;
}

bool f_PCA9685_setPWMFreq(setPWMFreq_args args) {
	int fd = args.fd? args.fd : -1;
	float freq = args.freq? args.freq : 50.0;

	if (fd < 0) {
		PCA9685_errno = 9;
		return false;
	}

	if (freq < 24.0 || freq > 1526.0) {
		PCA9685_errno = 7;
		return false;
	}

	float prescaleval = 25000000.0;
	prescaleval /= 4096.0;
	prescaleval /= freq;
	prescaleval -= 1.0;

	int prescale = (int)(prescaleval + 0.5);

	int oldmode = i2c_smbus_read_byte_data(fd, MODE1);
	int newmode = (oldmode & 0x7F) | 0x10;	// sleep

	if (i2c_smbus_write_byte_data(fd, MODE1, newmode) < 0) { // go to sleep
		PCA9685_errno = 5;
		return false;
	}

	if (i2c_smbus_write_byte_data(fd, PRESCALE, prescale) < 0) { // set the prescaler
		PCA9685_errno = 6;
		return false;
	}

	if (i2c_smbus_write_byte_data(fd, MODE1, oldmode) < 0) { // wake up
		PCA9685_errno = 7;
		return false;
	}

	usleep(5000); // wait for oscillator

	if (i2c_smbus_write_byte_data(fd, MODE1, oldmode | 0x80) < 0) { // turn on auto increment
		PCA9685_errno = 7;
		return false;
	}

	return true;
}

bool f_PCA9685_setPWM(setPWM_args args) {
	int fd = args.fd? args.fd : -1;
	int channel = args.channel? args.channel : 0;
	int on = args.on? args.on : 0;
	int off = args.off? args.off : 0;

	if (fd < 0) {
		PCA9685_errno = 9;
		return false;
	}

	if (channel < 0 || channel > 15) {
		PCA9685_errno = 5;
		return false;
	}

	if (on < 0 || on > 4095) {
		PCA9685_errno = 8;
		return false;
	}

	if (off < 0 || off > 4095) {
		PCA9685_errno = 8;
		return false;
	}

	if (on > off) {
		PCA9685_errno = 8;
		return false;
	}

	if (i2c_smbus_write_byte_data(fd, LED0_ON_L + 4 * channel, on & 0xFF) < 0) {
		PCA9685_errno = 3;
		return false;
	}

	if (i2c_smbus_write_byte_data(fd, LED0_ON_H + 4 * channel, on >> 8) < 0) {
		PCA9685_errno = 3;
		return false;
	}

	if (i2c_smbus_write_byte_data(fd, LED0_OFF_L + 4 * channel, off & 0xFF) < 0) {
		PCA9685_errno = 3;
		return false;
	}

	if (i2c_smbus_write_byte_data(fd, LED0_OFF_H + 4 * channel, off >> 8) < 0) {
		PCA9685_errno = 3;
		return false;
	}

	return true;
}

bool f_PCA9685_setPWMuS(setPWMuS_args args) {
	int fd = args.fd? args.fd : -1;
	int channel = args.channel? args.channel : 0;
	int uS = args.uS? args.uS : 0;

	if (fd < 0) {
		PCA9685_errno = 9;
		return false;
	}

	if (channel < 0 || channel > 15) {
		PCA9685_errno = 5;
		return false;
	}

	if (uS < 0 || uS > 200000) {
		PCA9685_errno = 8;
		return false;
	}

	int off = (uS * 4096) / 20000;

	return PCA9685_setPWM(fd, channel, 0, off);
}

bool PCA9685_close(int fd) {
	if (fd < 0) {
		PCA9685_errno = 9;
		return false;
	}

	close(fd);

	return true;
}

int PCA9685_errno;

const char* PCA9685_strerror(int err) {
	switch (err) {
		case 0:
			return "No error";
		case 1:
			return "Failed to open I2C device";
		case 2:
			return "Unable to set I2C slave address";
		case 3:
			return "Failed to write to I2C device";
		case 4:
			return "Failed to read from I2C device";
		case 5:
			return "Invalid channel number";
		case 6:
			return "Invalid pulse width";
		case 7:
			return "Invalid frequency";
		case 8:
			return "Invalid ON/OFF register values";
		case 9:
			return "Invalid I2C file descriptor";
		case 10:
			return "Invalid I2C address";
		case 11:
			return "Invalid I2C device file";
		default:
			return "Unknown error";
	}
}