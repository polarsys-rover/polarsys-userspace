#include <iostream>
#include <string>
#include <bitset>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <arpa/inet.h>

#include "i2c-dev.h"

#define ADS1115_I2C_LEN 2
#define ADS1115_REG_CONVERSION 0
#define ADS1115_REG_CONFIG 1

#define VOLTAGE_DIVIDER_RATIO 5.0
#define VOLTAGE_SCALE 4.096


class ADS1115Conf {
public:
	enum OS {
		CONVERSION_ONGOING = 0x0,
		NO_CONVERSION_ONGOING = 0x1,
	};

	enum InMux {
		AIN0_AIN1 = 0x0,
		AIN0_AIN3 = 0x1,
		AIN1_AIN3 = 0x2,
		AIN2_AIN3 = 0x3,
		AIN0_GND  = 0x4,
		AIN1_GND  = 0x5,
		AIN2_GND  = 0x6,
		AIN3_GND  = 0x7,
	};

	enum PGA {
		FS_6_144 = 0x0,
		FS_4_096 = 0x1,
		FS_2_048 = 0x2,
		FS_1_024 = 0x3,
		FS_0_512 = 0x4,
		FS_0_256 = 0x5,
		FS_0_256_2 = 0x6,
		FS_0_256_3 = 0x7,
	};

	static const uint8_t OS_SHIFT = 7;
	static const uint8_t OS_MASK = 0x1 << OS_SHIFT;

	static const uint8_t IN_MUX_SHIFT = 4;
	static const uint8_t IN_MUX_MASK = 0x7 << IN_MUX_SHIFT;

	static const uint8_t PGA_SHIFT = 1;
	static const uint8_t PGA_MASK = 0x7 << PGA_SHIFT;

	/* Create a new ADS1115Conf containing the default config of the
	   ADS1115. */
	ADS1115Conf()
	{
		m_raw[0] = 0x85;
		m_raw[1] = 0x83;
	}

	void setRaw(const uint8_t *raw)
	{
		m_raw[0] = raw[0];
		m_raw[1] = raw[1];
	}

	void getRaw(uint8_t *raw, bool start_conversion = false) const
	{
		if (start_conversion) {
			raw[0] = m_raw[0] | OS_MASK;
		} else {
			raw[0] = m_raw[0] & ~OS_MASK;
		}
		raw[1] = m_raw[1];
	}

	OS getOS(void) const
	{
		return (OS) ((m_raw[0] & OS_MASK) >> OS_SHIFT);
	}

	void setInMux(InMux value)
	{
		m_raw[0] = (m_raw[0] & ~IN_MUX_MASK) | (value << IN_MUX_SHIFT);
	}

	InMux getInMux(void) const
	{
		return (InMux) ((m_raw[0] & IN_MUX_MASK) >> IN_MUX_SHIFT);
	}

	void setPGA(PGA value)
	{
		m_raw[0] = (m_raw[0] & ~PGA_MASK) | (value << PGA_SHIFT);
	}

	PGA getPGA(void) const
	{
		return (PGA) ((m_raw[0] & PGA_MASK) >> PGA_SHIFT);
	}

private:
	uint8_t m_raw[2];
};

class ADS1115 {
public:
	ADS1115(std::string i2c_dev, unsigned char address);
	~ADS1115();
	bool init(void);

	bool ReadConf(ADS1115Conf &conf);
	bool WriteConf(const ADS1115Conf &conf, bool start_conversion = false);

	bool StartConversion(void);
	bool ReadConversionResult(uint16_t *res);
	bool WaitAndGetConversionResult(uint16_t *res);

private:
	bool ReadWithCheck(uint8_t command, uint8_t *buf);
	bool WriteWithCheck(uint8_t command, const uint8_t *buf);


	const std::string m_i2c_dev;
	int m_address;
	int m_fd;
};

ADS1115::ADS1115(std::string i2c_dev, unsigned char address)
: m_i2c_dev(i2c_dev),
  m_address(address),
  m_fd(-1)
{
}

ADS1115::~ADS1115()
{
	if (m_fd >= 0) {
		close(m_fd);
	}
}

bool ADS1115::init(void)
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

	ADS1115Conf conf;

	if (!ReadConf(conf)) {
		std::cerr << "Couldn't read config from ADS1115" << std::endl;
		return false;
	}

	/* We want to read from input 3. */
	conf.setInMux(ADS1115Conf::InMux::AIN3_GND);

	/* We want to be on the 4.096V scale. */
	conf.setPGA(ADS1115Conf::PGA::FS_4_096);

	if (!WriteConf(conf)) {
		std::cerr << "Couldn't write config to ADS1115" << std::endl;
		return false;
	}

	return true;
}

bool ADS1115::ReadWithCheck(uint8_t command, uint8_t *buf) {
        int ret = -1;

	ret = i2c_smbus_read_i2c_block_data(m_fd, command, ADS1115_I2C_LEN, buf);

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

bool ADS1115::WriteWithCheck(uint8_t command, const uint8_t *buf) {
	int ret;

	ret = i2c_smbus_write_i2c_block_data(m_fd, command, ADS1115_I2C_LEN, buf);

        if (ret != 0) {
                perror("i2c write");
                return false;
        }

        return true;

}

bool ADS1115::ReadConf(ADS1115Conf &conf)
{
	uint8_t raw[2];

	if (!ReadWithCheck(ADS1115_REG_CONFIG, raw)) {
		return false;
	}

	conf.setRaw(raw);

	return true;
}

bool ADS1115::WriteConf(const ADS1115Conf &conf, bool start_conversion)
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

bool ADS1115::StartConversion(void)
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

bool ADS1115::ReadConversionResult(uint16_t *res)
{
	uint8_t raw[2];

	if (!ReadWithCheck(ADS1115_REG_CONVERSION, raw)) {
		return false;
	}

	*res = (raw[0] << 8) | raw[1];

	return true;
}

bool ADS1115::WaitAndGetConversionResult(uint16_t *res)
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

const std::string I2C_DEV("/dev/i2c-1");
#define I2C_ADS1115_ADDR 0x48

void PrintADS1115Conf(const ADS1115Conf &conf)
{
	std::cout << "Input multiplexer: " << conf.getInMux() << std::endl;
	std::cout << "Gain amplifier: " << conf.getPGA() << std::endl;
	
}

float ConvertToVolts(uint16_t value, float scale, float voltage_divider) {
	return (value / 32767.0) * scale * voltage_divider;
}

// FIXME: what is the max voltage?
#define NUM_CELLS 3
#define VOLTAGE_MAX (4.2 * NUM_CELLS)
#define VOLTAGE_MIN (3.0 * NUM_CELLS)

float ConvertVoltageToPercent(float voltage)
{
	return 100 * ((voltage - VOLTAGE_MIN) / (VOLTAGE_MAX - VOLTAGE_MIN));
}

int main()
{
	std::cout << "Starting." << std::endl;
	ADS1115 ads(I2C_DEV, I2C_ADS1115_ADDR);
	std::cout << ads.init() << std::endl;
	ADS1115Conf conf;
	if (!ads.ReadConf(conf)) {
		std::cout << "There was an error" << std::endl;
		return 1;
	}

	PrintADS1115Conf(conf);

	while (1) {
		uint16_t res;
		ads.StartConversion();
		ads.WaitAndGetConversionResult(&res);

		float volts = ConvertToVolts(res, VOLTAGE_SCALE, VOLTAGE_DIVIDER_RATIO);
		std::cout << volts << " volts" << std::endl;
		float percent = ConvertVoltageToPercent(volts);
		std::cout << percent << " %" << std::endl;
		sleep(1);
	}
}


