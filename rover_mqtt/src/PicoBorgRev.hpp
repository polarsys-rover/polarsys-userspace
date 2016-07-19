#ifndef PicoBorgRev_HPP_
#define PicoBorgRev_HPP_

#include <string>

class PicoBorgRev {
public:
	PicoBorgRev(std::string i2c_dev, unsigned char address);
	virtual ~PicoBorgRev();

	bool init(void);
	void fini(void);

	bool SetMotors(float power_left, float power_right);



private:
	std::string m_i2c_dev;
	unsigned char m_address;
	int m_fd;

	PicoBorgRev(const PicoBorgRev &) = delete;
	void operator=(const PicoBorgRev &) = delete;

	int ReadWithCheck(uint8_t command, uint8_t *buf);
};

#endif /* SRC_PicoBorgRev_HPP_ */
