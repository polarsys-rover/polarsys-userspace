#ifndef SRC_PICAMREAL_HPP_
#define SRC_PICAMREAL_HPP_

#include "PiCam.hpp"
#include "raspicam/raspicam_cv.h"

class PiCamReal: public PiCam {
public:
	PiCamReal();
	virtual ~PiCamReal();

	virtual bool init();
	virtual void fini();

	virtual std::vector<unsigned char> capture();

private:
	raspicam::RaspiCam_Cv m_cam;
};

#endif /* SRC_PICAMREAL_HPP_ */
