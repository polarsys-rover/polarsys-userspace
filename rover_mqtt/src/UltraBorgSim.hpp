#ifndef ULTRABORGSIM_HPP_
#define ULTRABORGSIM_HPP_

#include <string>
#include <mutex>

#include "UltraBorg.hpp"

class UltraBorgSim : public UltraBorg {
public:
	UltraBorgSim(std::mutex &mutex);
	virtual ~UltraBorgSim();

	bool init(void);
	void fini(void);

	uint16_t GetDistance1(void);

private:
	uint64_t m_start_time;
	std::mutex &m_mutex;
};

#endif /* ULTRABORGSIM_HPP_ */
