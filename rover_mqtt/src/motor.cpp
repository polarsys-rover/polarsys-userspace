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

#define I2C_MAX_LEN 10
#define COMMAND_GET_ID 0x99

int main(int argc, char **argv)
{
	const std::string i2c_dev = "/dev/i2c-1";
	const unsigned char i2c_address = 0x36;

	UltraBorg ub(i2c_dev, i2c_address);
/*
	if (ret < 0) {
	    perror("ioctl set address");
	    //continue;
	}
	ret = i2c_smbus_read_i2c_block_data(fd, COMMAND_GET_ID, I2C_MAX_LEN, buf);
	//ret = fuck(fd, COMMAND_GET_ID, buf);
	if (ret < 0) {
	    perror("read block data");
	    //continue;
	}
	for (int i = 0; i < ret; i++) {
	    printf("%x ", buf[i]);
	}
	printf("\n");
    }*/

	if (!ub.init()) {
		std::cout << "Failed to init." << std::endl;
		return 1;
	}

	std::cout << "Inited" << std::endl;

    return 0;
}
