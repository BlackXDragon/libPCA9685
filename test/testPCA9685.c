/*
A test for the PCA9685 library.

It initialises the PCA9685 IC,
sets the frequency of the PWM signal to 50Hz
and sweeps the PWM signal from 1000uS to 2000uS
on channel 0.

To test the library, connect a servo motor to channel 0
or use an oscilloscope to measure the PWM signal.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <signal.h>

#include <PCA9685/PCA9685.h>

int fd;

void cleanup() {
	// Close the file descriptor
	PCA9685_close(fd);
}

int main() {
	// Initialise the PCA9685 IC
	fd = PCA9685_init();
	if (PCA9685_errno != 0) {
		printf("Error initialising PCA9685 IC: %s\n", PCA9685_strerror(PCA9685_errno));
		return 1;
	}

	// Reset the PCA9685 IC
	if (!PCA9685_reset(fd)) {
		printf("Error resetting PCA9685 IC: %s\n", PCA9685_strerror(PCA9685_errno));
		return 1;
	}

	// Set the frequency of the PWM signal
	if (!PCA9685_setPWMFreq(fd, 50.0)) {
		printf("Error setting PWM frequency: %s\n", PCA9685_strerror(PCA9685_errno));
		return 1;
	}
	
	// Set SIGINT handler
	signal(SIGINT, cleanup);

	// Sweep the PWM signal from 1000uS to 2000uS
	while (1) {
		for (int i = 1000; i <= 2000; i += 10) {
			if (!PCA9685_setPWMuS(fd, 0, i)) {
				printf("Error setting PWM signal: %s\n", PCA9685_strerror(PCA9685_errno));
				return 1;
			}

			usleep(10000);
		}

		for (int i = 2000; i >= 1000; i -= 10) {
			if (!PCA9685_setPWMuS(fd, 0, i)) {
				printf("Error setting PWM signal: %s\n", PCA9685_strerror(PCA9685_errno));
				return 1;
			}

			usleep(10000);
		}
	}

	cleanup();

	return 0;
}