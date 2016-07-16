#include "UltraBorg.hpp"

#include <linux/i2c-dev.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

UltraBorg::UltraBorg(std::string i2c_dev, unsigned char address)
: m_i2c_dev(i2c_dev), m_address(address), m_fd(-1)
{
}

bool UltraBorg::init(void)
{
	m_fd = open(m_i2c_dev.c_str(), O_RDWR);

	if (m_fd < 0) {
		perror("open i2c dev");
		return false;
	}

	int ret = ioctl(m_fd, I2C_SLAVE, m_address);
	if (ret < 0) {
		perror("ioctl set slave address");
		return false;
	}


	return true;
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

