#ifndef SENSORSTHREAD_HPP_
#define SENSORSTHREAD_HPP_

#include "RobotSensorValues.hpp"
#include "SelectLoopThread.hpp"
#include <RTIMULib.h>

#include "UltraBorgReal.hpp"

class SensorsThread : public SelectLoopThread {
public:
	SensorsThread(RobotSensorValues &sensor_values, UltraBorg &ultra_borg);
	virtual ~SensorsThread();

	virtual void timeout(void);

private:
	RobotSensorValues &m_sensor_values;
	RTIMUSettings m_imu_settings;
	std::unique_ptr<RTIMU> m_imu;
	UltraBorg &m_ultra_borg;

	SensorsThread(const SensorsThread&) = delete;
	void operator=(const SensorsThread&) = delete;
};

#endif /* SENSORSTHREAD_HPP_ */
