#ifndef SRC_PICAM_HPP_
#define SRC_PICAM_HPP_

#include <vector>

class PiCam {
public:
	PiCam();
	virtual ~PiCam();

	virtual bool init() = 0;
	virtual void fini() = 0;

	virtual std::vector<unsigned char> capture() = 0;
};

#endif /* SRC_PICAM_HPP_ */
