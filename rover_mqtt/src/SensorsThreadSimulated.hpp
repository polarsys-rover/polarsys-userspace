#ifndef SENSORSTHREADSIMULATED_HPP_
#define SENSORSTHREADSIMULATED_HPP_

#include "RobotSensorValues.hpp"
#include "SelectLoopThread.hpp"

class SensorsThreadSimulated : public SelectLoopThread {
public:
	SensorsThreadSimulated(RobotSensorValues &sensor_values);
	virtual ~SensorsThreadSimulated();

	virtual void timeout(void);

private:

	RobotSensorValues &m_sensor_values;
};

#endif /* SENSORSTHREADSIMULATED_HPP_ */
