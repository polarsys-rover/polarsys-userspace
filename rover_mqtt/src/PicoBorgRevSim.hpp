#ifndef PICOBORGREVSIM_HPP_
#define PICOBORGREVSIM_HPP_

#include <string>

#include "PicoBorgRev.hpp"

class PicoBorgRevSim : public PicoBorgRev {
public:
	bool init(void);
	void fini(void);

	bool SetMotors(float power_left, float power_right);

};

#endif /* PICOBORGREVSIM_HPP_ */
