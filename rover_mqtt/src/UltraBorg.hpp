#ifndef ULTRABORG_HPP_
#define ULTRABORG_HPP_

#include <string>

class UltraBorg {
public:
	UltraBorg(std::string i2c_dev, unsigned char address);
	virtual ~UltraBorg();

	bool init(void);
	void fini(void);

	uint16_t GetDistance1(void);



private:
	std::string m_i2c_dev;
	unsigned char m_address;
	int m_fd;

	UltraBorg(const UltraBorg &) = delete;
	void operator=(const UltraBorg &) = delete;

	int ReadWithCheck(uint8_t command, uint8_t *buf);
};

#endif /* SRC_ULTRABORG_HPP_ */
