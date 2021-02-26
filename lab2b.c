#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "wiringPi.h"
#include "linux/gpio.h"
#include "sys/ioctl.h"
#include "string.h"

int main(int argc, char **argv)
{
	unsigned int pin_mask, pin_set, i;
	
	if (argc != 3) {
		printf("USAGE: ./set_led (pin mask [0x00 - 0xFF]) (off [0] or on [1])\n");
		exit(-1);
	}

	// Check if input pin mask is valid
	if (!(strncmp("0x",argv[1],2))) {
		if (strlen(argv[1]) < 3) {
			printf("ERROR: Invalid pin mask\n");
			exit(-2);
		} else {
			for (i = 2; i < strlen(argv[1]); i++) {
				// Check if each character is not valid hex value (between '0' & '7', 'a' & 'f', 'A' & 'F')
				if (!(((argv[1][i] <= '9') && (argv[1][i] >= '0'))
							|| ((argv[1][i] <= 'f') && (argv[1][i] >= 'a'))
							|| ((argv[1][i] <= 'F') && (argv[1][i] >= 'A')))) {
					printf("ERROR: Invalid pin mask (hexadecimal)\n");
					exit(-2);
				}
			}
		}
	} else if (argv[1][0] == '0') {
		for (i = 0; i < strlen(argv[1]); i++) {
			if (!((argv[1][i] <= '7') && (argv[1][i] >= '0'))) {
				printf("ERROR: Invalid pin mask (base 8)\n");
				exit(-3);
			}
		}
	} else {
		for (i = 0; i < strlen(argv[1]); i++) {
			if (!((argv[1][i] <= '9') && (argv[1][i] >= '0'))) {
				printf("ERROR: Invalid pin mask (base 10)\n");
				exit(-3);
			}
		}
	}

	// Determine base of input pin mask
	if (argv[1][0] == '0') {
		if ((argv[1][1] == 'x') || (argv[1][1] == 'X'))
			pin_mask = strtol(argv[1], NULL, 16);
		else
			pin_mask = strtol(argv[1], NULL, 8);
	} else {
		pin_mask = atoi(argv[1]);
	}

	// Check if input is in valid range
	if (pin_mask > 0xFF) {
		printf("ERROR: Pin value exceeds valid input\n");
		exit(-4);
	}

	// Check if value is valid
	pin_set = atoi(argv[2]);
	if ((pin_set != 0) && (pin_set != 1)) {
		printf("ERROR: Pin set value not valid\n");
		exit(-5);
	}


	// Initialize GPIO and set the driving value
	wiringPiSetup();
	
	// Drive the appropriate pins high or low depending on input
	for (i = 0; i < 8; i++) {
		if (pin_mask & (1 << i)) {
			pinMode(i, OUTPUT);
			digitalWrite(i, pin_set);
		}
	}

	// USE FOR DEBUGGING
	//printf("pin_mask: %#4x\npin_set: %4u\n",pin_mask,pin_set);

	return 0;
}
