#ifndef PICOBORGREVREAL_HPP_
#define PICOBORGREVREAL_HPP_

#include <string>

#include "PicoBorgRev.hpp"

class PicoBorgRevReal : public PicoBorgRev {
public:
	PicoBorgRevReal(std::string i2c_dev, unsigned char address);
	virtual ~PicoBorgRevReal();

	bool init(void);
	void fini(void);

	bool SetMotors(float power_left, float power_right);



private:
	std::string m_i2c_dev;
	unsigned char m_address;
	int m_fd;

	PicoBorgRevReal(const PicoBorgRevReal &) = delete;
	void operator=(const PicoBorgRevReal &) = delete;

	int ReadWithCheck(uint8_t command, uint8_t *buf);
};

#endif /* PICOBORGREVREAL_HPP_ */
