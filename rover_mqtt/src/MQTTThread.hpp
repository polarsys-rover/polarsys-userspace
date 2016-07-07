#ifndef SRC_MQTTTHREAD_HPP_
#define SRC_MQTTTHREAD_HPP_

#include "RobotSensorValues.hpp"

class MQTTThread {
public:
	MQTTThread(const RobotSensorValues &sensor_values);
	virtual ~MQTTThread();

	void operator()(void);

private:
	void init(void);

	const RobotSensorValues &m_sensor_values;
};

#endif /* SRC_MQTTTHREAD_HPP_ */
