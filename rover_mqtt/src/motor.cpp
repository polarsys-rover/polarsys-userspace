/*
 * imu.c
 *
 * Test the values of rpi sensor hat
 *
 *  Created on: Jul 7, 2016
 *      Author: francis
 */

#include <iostream>
#include <cstdio>

#include "UltraBorg.hpp"

#include <unistd.h>

#define I2C_MAX_LEN 10
#define COMMAND_GET_ID 0x99

int main(int argc, char **argv) {
    const std::string i2c_dev = "/dev/i2c-1";
    const unsigned char i2c_address = 0x36;

    UltraBorg ub(i2c_dev, i2c_address);

    if (!ub.init()) {
	std::cout << "Failed to init." << std::endl;
	return 1;
    }

    std::cout << "Inited" << std::endl;

    while (1) {
	uint16_t distance = ub.GetDistance1();

	printf("Distance = %04x %d\n", distance, distance);
	sleep(1);
    }

    return 0;
}
