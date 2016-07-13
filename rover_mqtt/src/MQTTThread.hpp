#ifndef SRC_MQTTTHREAD_HPP_
#define SRC_MQTTTHREAD_HPP_

#include "RobotSensorValues.hpp"
#include "SelectLoopThread.hpp"

class MqttRoverServer;

namespace PolarsysRover {
    class RoverSensors;
    class Acceleration;
};

class MQTTThread : public SelectLoopThread {
public:
	MQTTThread(const RobotSensorValues &sensor_values);
	virtual ~MQTTThread();

protected:
	virtual void init(void);
	virtual void fini(void);
	virtual void timeout(void);
private:
	const RobotSensorValues &m_sensor_values;
	MqttRoverServer *m_server;

	PolarsysRover::RoverSensors *m_protobuf_sensors;
	PolarsysRover::Acceleration *m_protobuf_accel;
};

#endif /* SRC_MQTTTHREAD_HPP_ */
