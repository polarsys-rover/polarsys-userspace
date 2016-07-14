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
: SelectLoopThread("mqtt-thread", 1000),
  m_sensor_values(sensor_values),
  m_mqtt_interface(mqtt_interface),
  m_protobuf_sensors(new PolarsysRover::RoverSensors()),
  m_protobuf_accel(new PolarsysRover::Acceleration())
{
    m_protobuf_sensors->set_allocated_accel(m_protobuf_accel);
}

void SensorsPublishThread::timeout (void)
{
    std::string encoded;

    Accel accelValues = m_sensor_values.getAccel();

    m_protobuf_accel->set_x(accelValues.x);
    m_protobuf_accel->set_y(accelValues.y);
    m_protobuf_accel->set_z(accelValues.z);

    m_protobuf_sensors->SerializeToString(&encoded);

    m_mqtt_interface.publish(MQTT_SENSORS_TOPIC, encoded.length(),
			     encoded.c_str());
}

