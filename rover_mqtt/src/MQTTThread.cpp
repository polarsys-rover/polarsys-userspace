#include "MQTTThread.hpp"
#include <unistd.h>

#include <iostream>
#include <thread>

#include <mosquittopp.h>

#include <sensors.pb.h>

#define MQTT_BROKER_ADDR "127.0.0.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_SENSORS_TOPIC "/polarsys-rover/sensors"



class MqttRoverServer: public mosqpp::mosquittopp {
    virtual void on_connect(int rc) {
	std::cout << "on_connect: " << rc << std::endl;
    }
    virtual void on_disconnect() {
	std::cout << "on_disconnect" << std::endl;
    }
    virtual void on_message(const mosquitto_message *message) {
	std::cout << "on_message: " << message << std::endl;
    }
};

MQTTThread::MQTTThread(const RobotSensorValues &sensor_values)
: SelectLoopThread("mqtt-thread", 1000), m_sensor_values(sensor_values),
  m_protobuf_sensors(NULL), m_protobuf_accel(NULL)
{
}

MQTTThread::~MQTTThread()
{
	// TODO Auto-generated destructor stub
}

void MQTTThread::timeout (void)
{
	std::string encoded;

    Accel accelValues = m_sensor_values.getAccel();

    m_protobuf_accel->set_x(accelValues.x);
    m_protobuf_accel->set_y(accelValues.y);
    m_protobuf_accel->set_z(accelValues.z);

    m_protobuf_sensors->SerializeToString(&encoded);

    m_server->publish(NULL, MQTT_SENSORS_TOPIC, encoded.length(),
	    encoded.c_str(), 0, false);
}

void MQTTThread::init(void)
{
    int ret;
    mosqpp::lib_init();
    m_server = new MqttRoverServer();
    ret = m_server->connect(MQTT_BROKER_ADDR, MQTT_BROKER_PORT, 7200);

    std::cout << "Starting MQTT server" << std::endl;

    if (ret != MOSQ_ERR_SUCCESS) {
	std::cerr << "Could not connect, ret = " << ret << std::endl;
    }

    /* Start the mosquitto event loop. */
    m_server->loop_start();

    m_protobuf_sensors = new PolarsysRover::RoverSensors();
    m_protobuf_accel = new PolarsysRover::Acceleration();
    m_protobuf_sensors->set_allocated_accel(m_protobuf_accel);
}

void MQTTThread::fini(void)
{
    std::cout << "Asking the mosquitto library thread to stop" << std::endl;
    // FIXME: It doesn't seem to work without force=1
    m_server->loop_stop(1);
}
