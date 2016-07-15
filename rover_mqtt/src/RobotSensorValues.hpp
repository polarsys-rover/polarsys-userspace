#ifndef ROBOTSENSORVALUES_HPP_
#define ROBOTSENSORVALUES_HPP_

#include <cstdint>
#include <mutex>
#include <RTIMULib.h>

class RobotSensorValues {
public:
	RobotSensorValues();

	RTIMU_DATA getData(void) const;
	void setData(const RTIMU_DATA &data);

private:
	RTIMU_DATA m_data;

	mutable std::mutex m_mutex;
};

#endif /* ROBOTSENSORVALUES_HPP_ */
