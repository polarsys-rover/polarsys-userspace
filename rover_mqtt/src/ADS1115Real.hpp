#ifndef ADS1115REAL_HPP_
#define ADS1115REAL_HPP_

#include <string>
#include <mutex>

#include "ADS1115.hpp"

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

	OS getOS() const
	{
		return (OS) ((m_raw[0] & OS_MASK) >> OS_SHIFT);
	}

	void setInMux(InMux value)
	{
		m_raw[0] = (m_raw[0] & ~IN_MUX_MASK) | (value << IN_MUX_SHIFT);
	}

	InMux getInMux() const
	{
		return (InMux) ((m_raw[0] & IN_MUX_MASK) >> IN_MUX_SHIFT);
	}

	void setPGA(PGA value)
	{
		m_raw[0] = (m_raw[0] & ~PGA_MASK) | (value << PGA_SHIFT);
	}

	PGA getPGA() const
	{
		return (PGA) ((m_raw[0] & PGA_MASK) >> PGA_SHIFT);
	}

private:
	uint8_t m_raw[2];
};


class ADS1115Real : public ADS1115 {
public:
	ADS1115Real(std::mutex &mutex, const std::string &i2c_dev, unsigned char address);
	virtual ~ADS1115Real();

	virtual bool init();
	virtual void fini();

	virtual bool initialized () const {
		return m_fd >= 0;
	}

	/* Start an ADC conversion. */
	virtual bool StartConversion();

	/* Get the battery voltage in millivolts.  */
	virtual bool WaitAndGetConversionResult(uint16_t *res);

private:
	std::string m_i2c_dev;
	unsigned char m_address;
	int m_fd;
	std::mutex &m_mutex;

	ADS1115Real(const ADS1115Real &) = delete;
	void operator=(const ADS1115Real &) = delete;

	bool ReadConf(ADS1115Conf &conf);
	bool WriteConf(const ADS1115Conf &conf, bool start_conversion = false);

	bool ReadConversionResult(uint16_t *res);

	bool ReadWithCheck(uint8_t command, uint8_t *buf);
	bool WriteWithCheck(uint8_t command, const uint8_t *buf);

};

#endif /* ADS1115REAL_HPP_ */
