#ifndef PICOBORGREVSIM_HPP_
#define PICOBORGREVSIM_HPP_

#include <string>
#include <mutex>

#include "PicoBorgRev.hpp"

class PicoBorgRevSim : public PicoBorgRev {
public:
	PicoBorgRevSim(std::mutex &mutex);
	bool init(void);
	void fini(void);

	bool SetMotors(float power_left, float power_right);
private:
	std::mutex &m_mutex;

};

#endif /* PICOBORGREVSIM_HPP_ */
