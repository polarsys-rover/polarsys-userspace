#include "i2c-dev.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "ADS1115Real.hpp"
#include <iostream>
#include "tracepoints.h"

#define ADS1115_I2C_LEN 2

#define ADS1115_REG_CONVERSION 0
#define ADS1115_REG_CONFIG 1


ADS1115Real::ADS1115Real(std::mutex &mutex, const std::string &i2c_dev,
		unsigned char address) :
		m_i2c_dev(i2c_dev), m_address(address), m_fd(-1), m_mutex(mutex) {
}

bool ADS1115Real::init() {
	m_fd = open(m_i2c_dev.c_str(), O_RDWR);
        ADS1115Conf conf;
	int ret;

	if (m_fd < 0) {
		perror("open i2c dev");
		goto fail;
	}

	ret = ioctl(m_fd, I2C_SLAVE, m_address);
	if (ret < 0) {
		perror("ioctl set slave address");
		goto fail;
	}

        if (!ReadConf(conf)) {
                std::cerr << "Couldn't read config from ADS1115" << std::endl;
		goto fail;
        }

        /* We want to read from input 3. */
        conf.setInMux(ADS1115Conf::InMux::AIN3_GND);

        /* We want to be on the 4.096V scale. */
        conf.setPGA(ADS1115Conf::PGA::FS_4_096);

        if (!WriteConf(conf)) {
                std::cerr << "Couldn't write config to ADS1115" << std::endl;
		goto fail;
        }

        return true;

fail:
	if (m_fd >= 0) {
		close(m_fd);
		m_fd = -1;
	}

	return false;
}

/*
 * Returns the number of bytes read (up to I2C_MAX_LEN).
 */
bool ADS1115Real::ReadWithCheck(uint8_t command, uint8_t *buf) {
	int ret = -1;

	tracepoint(rover_mqtt, ReadWithCheck_begin);
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		ret = i2c_smbus_read_i2c_block_data(m_fd, command,
				ADS1115_I2C_LEN, buf);
	}
	tracepoint(rover_mqtt, ReadWithCheck_end);

	if (ret < 0) {
		perror("i2c read");
		return false;
	}

	if (ret != ADS1115_I2C_LEN) {
		std::cerr << "Wrong number of bytes read from I2C." << std::endl;
		return false;
	}
	return true;

}

bool ADS1115Real::WriteWithCheck(uint8_t command, const uint8_t *buf) {
        int ret;

	tracepoint(rover_mqtt, WriteWithCheck_begin);
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		ret = i2c_smbus_write_i2c_block_data(m_fd, command, ADS1115_I2C_LEN, buf);
	}
	tracepoint(rover_mqtt, WriteWithCheck_end);

        if (ret != 0) {
                perror("i2c write");
                return false;
        }

        return true;

}

void ADS1115Real::fini() {
	if (m_fd >= 0) {
		close(m_fd);
		m_fd = 1;
	}
}

ADS1115Real::~ADS1115Real() {
	fini();
}

bool ADS1115Real::ReadConf(ADS1115Conf &conf)
{
        uint8_t raw[2];

        if (!ReadWithCheck(ADS1115_REG_CONFIG, raw)) {
                return false;
        }

        conf.setRaw(raw);

        return true;
}

bool ADS1115Real::WriteConf(const ADS1115Conf &conf, bool start_conversion)
{
        /* Override the single-shot conversion trigger bit, since it does not
           have the same meaning as the OS bit that we read. */
        uint8_t raw[2];

        conf.getRaw(raw, start_conversion);

        if (!WriteWithCheck(ADS1115_REG_CONFIG, raw)) {
                return false;
        }

        ADS1115Conf conf_check;

        if (!ReadConf(conf_check)) {
                return false;
        }

        uint8_t raw_check[2];

        conf_check.getRaw(raw_check, start_conversion);

        if (raw[0] != raw_check[0] || raw[1] != raw_check[1]) {
                std::cerr << "Read verification failed." << std::endl;
                return false;
        }

        return true;
}

bool ADS1115Real::StartConversion()
{
        ADS1115Conf conf;

        /* Because of the behavior of the OS bit, reading and writing the conf
           will trigger a conversion, if the chip isn't performing one right
           now. */
        if (!ReadConf(conf)) {
                return false;
        }

        if (!WriteConf(conf, true)) {
                return false;
        }

        return true;
}

bool ADS1115Real::ReadConversionResult(uint16_t *res)
{
        uint8_t raw[2];

        if (!ReadWithCheck(ADS1115_REG_CONVERSION, raw)) {
                return false;
        }

        *res = (raw[0] << 8) | raw[1];

        return true;
}

bool ADS1115Real::WaitAndGetConversionResult(uint16_t *res)
{
        ADS1115Conf conf;
        int i;

        for (i = 0; i < 10; i++) {
                if (!ReadConf(conf)) {
                        return false;
                }

                if (conf.getOS() == ADS1115Conf::OS::NO_CONVERSION_ONGOING) {
                        break;
                }

                usleep (1000);
        }

        return ReadConversionResult(res);
}

