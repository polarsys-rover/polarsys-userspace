#include "PicoBorgRev.hpp"

#include <linux/i2c-dev-user.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>

// Defines stolen from the PicoBorgRev PIC code
#define I2C_ID_SERVO_USM		(0x36)
#define I2C_ID_PICOBORG_REV 0x15

#define COMMAND_GET_FILTER_USM1	(41)	// Get the filtered time measured by ultrasonic #1 in us (0 for no detection)
#define COMMAND_GET_ID			(0x99)	// Get the board identifier

#define COMMAND_SET_A_FWD 0x03
#define COMMAND_SET_A_REV 0x04
#define COMMAND_SET_B_FWD 0x06
#define COMMAND_SET_B_REV 0x07

#define PicoBorgRev_I2C_MAX_LEN 4

PicoBorgRev::PicoBorgRev(std::string i2c_dev, unsigned char address)
: m_i2c_dev(i2c_dev),
  m_address(address),
  m_fd(-1) {
}

bool PicoBorgRev::init(void)
{
    m_fd = open(m_i2c_dev.c_str(), O_RDWR);
    uint8_t buf[PicoBorgRev_I2C_MAX_LEN];

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

    if (buf[1] != I2C_ID_PICOBORG_REV) {
	fprintf(stderr, "Wrong ID read, PicoBorgRev id = 0x%x, id read from device = 0x%x\n",
		I2C_ID_PICOBORG_REV, buf[1]);
	return false;
    }

    return true;
}
/*
 * Returns the number of bytes read (up to I2C_MAX_LEN).
 */
int PicoBorgRev::ReadWithCheck(uint8_t command, uint8_t *buf)
{
    int ret = i2c_smbus_read_i2c_block_data(m_fd, command,
	    PicoBorgRev_I2C_MAX_LEN, buf);
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

void PicoBorgRev::fini(void)
{
    if (m_fd >= 0) {
	close(m_fd);
    }
}

PicoBorgRev::~PicoBorgRev() {
    fini();
}

bool PicoBorgRev::SetMotors(float power_left, float power_right) {
    int ret;
    bool res = true;

    const uint8_t command_left = power_left < 0 ? COMMAND_SET_A_REV : COMMAND_SET_A_FWD;
    const uint8_t command_right = power_right > 0 ? COMMAND_SET_B_REV : COMMAND_SET_B_FWD;

    power_left = std::min(std::abs(power_left), 1.0f);
    power_right = std::min(std::abs(power_right), 1.0f);

    const uint8_t max_power = 200;

    const uint8_t pwm_left = max_power * power_left;
    const uint8_t pwm_right = max_power * power_right;

    ret = i2c_smbus_write_byte_data(m_fd, command_left, pwm_left);
    if (ret < 0) {
	perror("write byte data");
	res = false;
    }

    ret = i2c_smbus_write_byte_data(m_fd, command_right, pwm_right);
    if (ret < 0) {
	perror("write byte data");
	res = false;
    }

    return res;
}
