#ifndef PISENSEHATSIM_HPP_
#define PISENSEHATSIM_HPP_

#include "PiSenseHat.hpp"
#include <mutex>

class PiSenseHatSim : public PiSenseHat {
public:
	PiSenseHatSim(std::mutex &mutex);
	virtual ~PiSenseHatSim();

	bool init(void);
	void fini(void);

	RTIMU_DATA ReadValues(void);

private:
	uint64_t m_start_time;
	std::mutex &m_mutex;
};

#endif /* PISENSEHATSIM_HPP_ */
