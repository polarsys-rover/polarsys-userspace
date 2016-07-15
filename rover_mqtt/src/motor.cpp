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
#include <linux/i2c-dev-user.h>

#define I2C_MAX_LEN 4
#define COMMAND_GET_ID 0x99

/* Returns the number of read bytes */
static inline __s32 fuck(int file, __u8 command, __u8 *values)
{
	union i2c_smbus_data data;
	int i;
	if (i2c_smbus_access(file,I2C_SMBUS_READ,command,
	                     I2C_SMBUS_BLOCK_DATA,&data))
		return -1;
	else {
		for (i = 1; i <= data.block[0]; i++)
			values[i-1] = data.block[i];
		return data.block[0];
	}
}

int main(int argc, char **argv)
{
    (void) argc; (void) argv;
    __u8 buf[I2C_MAX_LEN];

    FILE *i2c = fopen("/dev/i2c-1", "rw");
    int fd = fileno(i2c);
    //for (__u8 addr = 0x3; addr < 0x77; addr++) {
    __u8 addr = 0x44;
    {
	printf("scanning addr=0x%x\n", addr);
	int ret = ioctl(fd, I2C_SLAVE, addr);
	if (ret < 0) {
	    perror("ioctl set address");
	    //continue;
	}
	ret = fuck(fd, COMMAND_GET_ID, buf);
	if (ret < 0) {
	    perror("read block data");
	    //continue;
	}
	for (int i = 0; i < ret; i++) {
	    printf("%x ", buf[i]);
	}
	printf("\n");
    }

    return 0;
}
