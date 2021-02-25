#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "wiringPi.h"
#include "linux/gpio.h"
#include "sys/ioctl.h"
#include "string.h"

int main(int argc, char **argv)
{
	unsigned char pin_mask, pin_set, i;
	
	if (argc != 3) {
		printf("USAGE: ./set_led (pin mask [0x00 - 0xFF]) (off [0] or on [1])\n");
		exit(-1);
	}

	// Check that input values are valid
	for (i = 0; i < strlen(argv[1]); i++) {
		if (argv[1][i] > '7' || argv[1][i] < '0') {
			printf("ERROR: Invalid pin number\n");
			exit(-2);
		}
	}

	for (i = 0; i < strlen(argv[2]); i++) {
		if ((argv[2][i] != '0') && (argv[2][i] != '1')) {
			printf("ERROR: Invalid setting\n");
			exit(-3);
		}
	}
	
	// Determine base of input pin
	if (argv[1][0] == '0') {
	       	if ((argv[1][1] == 'x') || (argv[1][1] == 'X'))
			pin_mask = strtol(argv[1], NULL, 16);
		else
			pin_mask = strtol(argv[1], NULL, 8);
	} else {
		pin_mask = atoi(argv[2]);
	}

	wiringPiSetup();

	pin_set = atoi(argv[2]);

	for (i = 0; i < 8; i++) {
		if (pin_mask & (1 << i)) {
			pinMode(i, OUTPUT);
			digitalWrite(i, pin_set);
		}
	}

	while (1);

	return 0;
}
