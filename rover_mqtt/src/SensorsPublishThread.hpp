#ifndef SRC_SENSORSPUBLISHTHREAD_HPP_
#define SRC_SENSORSPUBLISHTHREAD_HPP_

#include "RobotSensorValues.hpp"
#include "SelectLoopThread.hpp"
#include "MqttInterface.hpp"
#include "sensors.pb.h"

#include <memory>


class SensorsPublishThread : public SelectLoopThread {
public:
	SensorsPublishThread(const RobotSensorValues &sensor_values,
			     MqttInterface &mqtt_interface);

protected:
	virtual void timeout(void);

private:
	const RobotSensorValues &m_sensor_values;
	MqttInterface &m_mqtt_interface;

	std::unique_ptr<PolarsysRover::RoverSensors> m_protobuf_sensors;
	PolarsysRover::Acceleration *m_protobuf_accel;
};

#endif /* SRC_SENSORSPUBLISHTHREAD_HPP_ */
