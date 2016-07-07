#ifndef SENSORSTHREADSIMULATED_HPP_
#define SENSORSTHREADSIMULATED_HPP_

#include "RobotSensorValues.hpp"

class SensorsThreadSimulated {
public:
	SensorsThreadSimulated(RobotSensorValues &sensor_values);
	virtual ~SensorsThreadSimulated();

	void operator()(void);

private:

	RobotSensorValues &m_sensor_values;
};

#endif /* SENSORSTHREADSIMULATED_HPP_ */
