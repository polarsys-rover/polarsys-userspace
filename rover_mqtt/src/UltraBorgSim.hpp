#ifndef ULTRABORGSIM_HPP_
#define ULTRABORGSIM_HPP_

#include <string>

#include "UltraBorg.hpp"

class UltraBorgSim : public UltraBorg {
public:
	UltraBorgSim();
	virtual ~UltraBorgSim();

	bool init(void);
	void fini(void);

	uint16_t GetDistance1(void);

private:
	uint64_t m_start_time;
};

#endif /* ULTRABORGSIM_HPP_ */
