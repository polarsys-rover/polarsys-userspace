#include "UltraBorg.hpp"

#include <linux/i2c-dev-user.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>

// Defines stolen from the UltraBorg PIC code
#define I2C_ID_SERVO_USM		(0x36)
#define COMMAND_GET_FILTER_USM1	(41)	// Get the filtered time measured by ultrasonic #1 in us (0 for no detection)
#define COMMAND_GET_ID			(0x99)	// Get the board identifier

#define ULTRABORG_I2C_MAX_LEN 4

UltraBorg::UltraBorg(std::string i2c_dev, unsigned char address)
: m_i2c_dev(i2c_dev),
  m_address(address),
  m_fd(-1) {
}

bool UltraBorg::init(void)
{
    m_fd = open(m_i2c_dev.c_str(), O_RDWR);
    uint8_t buf[ULTRABORG_I2C_MAX_LEN];

    if (m_fd < 0) {
	perror("open i2c dev");
	return false;
    }

    int ret = ioctl(m_fd, I2C_SLAVE, m_address);
    if (ret < 0) {
	perror("ioctl set slave address");
	return false;
    }

    ret = ReadWithCheck(COMMAND_GET_ID, buf);
    if (ret < 0) {
	return false;
    }

    if (buf[1] != I2C_ID_SERVO_USM) {
	fprintf(stderr, "Wrong ID read, UltraBorg id = 0x%x, id read from device = 0x%x\n",
		I2C_ID_SERVO_USM, buf[1]);
	return false;
    }

    return true;
}
/*
 * Returns the number of bytes read (up to I2C_MAX_LEN).
 */
int UltraBorg::ReadWithCheck(uint8_t command, uint8_t *buf)
{
    int ret = i2c_smbus_read_i2c_block_data(m_fd, command,
	    ULTRABORG_I2C_MAX_LEN, buf);
    if (ret < 0) {
	perror("i2c read");
	return -1;
    }

    if (ret < 1) {
	std::cerr << "0 bytes read from I2C." << std::endl;
	return -1;
    }

    return ret;

}

void UltraBorg::fini(void)
{
    if (m_fd >= 0) {
	close(m_fd);
    }
}

UltraBorg::~UltraBorg() {
    fini();
}

uint16_t UltraBorg::GetDistance1(void) {
    uint8_t buf[ULTRABORG_I2C_MAX_LEN];
    uint16_t distance;

    int ret = ReadWithCheck(COMMAND_GET_FILTER_USM1, buf);
    if (ret < 0) {
	return 0;
    }

    distance = (buf[1] << 8) + buf[2];

    return distance;
}
