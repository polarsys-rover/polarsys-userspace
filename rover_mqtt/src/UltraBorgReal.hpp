#ifndef ULTRABORGREAL_HPP_
#define ULTRABORGREAL_HPP_

#include <string>
#include <mutex>

#include "UltraBorg.hpp"

class UltraBorgReal : public UltraBorg {
public:
	UltraBorgReal(std::mutex &mutex, std::string i2c_dev, unsigned char address);
	virtual ~UltraBorgReal();

	bool init(void);
	void fini(void);

	uint16_t GetDistance1(void);

private:
	std::string m_i2c_dev;
	unsigned char m_address;
	int m_fd;
	std::mutex &m_mutex;

	UltraBorgReal(const UltraBorgReal &) = delete;
	void operator=(const UltraBorgReal &) = delete;

	int ReadWithCheck(uint8_t command, uint8_t *buf);
};

#endif /* ULTRABORGREAL_HPP_ */
