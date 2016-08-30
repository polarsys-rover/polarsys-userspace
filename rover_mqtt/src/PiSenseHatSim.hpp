#ifndef PISENSEHATSIM_HPP_
#define PISENSEHATSIM_HPP_

#include "PiSenseHat.hpp"

class PiSenseHatSim : public PiSenseHat {
public:
	PiSenseHatSim();
	virtual ~PiSenseHatSim();

	bool init(void);
	void fini(void);

	RTIMU_DATA ReadValues(void);
};

#endif /* PISENSEHATSIM_HPP_ */
