#include <unistd.h>

#include <iostream>
#include <thread>

#include <mosquittopp.h>

#include <sensors.pb.h>
#include <SensorsPublishThread.hpp>

#define MQTT_BROKER_ADDR "127.0.0.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_SENSORS_TOPIC "/polarsys-rover/sensors"

SensorsPublishThread::SensorsPublishThread(const RobotSensorValues &sensor_values,
					   MqttInterface &mqtt_interface)
: SelectLoopThread("mqtt-thread", 200),
  m_sensor_values(sensor_values),
  m_mqtt_interface(mqtt_interface),
  m_protobuf_sensors(new PolarsysRover::RoverSensors())
{
    m_protobuf_sensors->set_allocated_accel(new PolarsysRover::Acceleration());
}

void SensorsPublishThread::timeout (void)
{
    std::string encoded;

    RTIMU_DATA data = m_sensor_values.getData();

    m_protobuf_sensors->Clear();
    if (data.accelValid) {
	auto accel = m_protobuf_sensors->mutable_accel();
	accel->set_x(data.accel.x());
	accel->set_y(data.accel.y());
	accel->set_z(data.accel.z());
    }
    if (data.gyroValid) {
	auto gyro = m_protobuf_sensors->mutable_gyro();
	gyro->set_x(data.gyro.x());
	gyro->set_y(data.gyro.y());
	gyro->set_z(data.gyro.z());
    }
    if (data.compassValid) {
	auto compass = m_protobuf_sensors->mutable_compass();
	compass->set_x(data.compass.x());
	compass->set_y(data.compass.y());
	compass->set_z(data.compass.z());
    }
    m_protobuf_sensors->SerializeToString(&encoded);

    m_mqtt_interface.publish(MQTT_SENSORS_TOPIC, encoded.length(),
			     encoded.c_str());
}

