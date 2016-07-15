#ifndef SENSORSTHREAD_HPP_
#define SENSORSTHREAD_HPP_

#include "RobotSensorValues.hpp"
#include "SelectLoopThread.hpp"
#include <RTIMULib.h>

class SensorsThread : public SelectLoopThread {
public:
	SensorsThread(RobotSensorValues &sensor_values);
	virtual ~SensorsThread();

	virtual void timeout(void);

private:

	RobotSensorValues &m_sensor_values;
	RTIMUSettings m_imu_settings;
	std::unique_ptr<RTIMU> m_imu;
};

#endif /* SENSORSTHREAD_HPP_ */
