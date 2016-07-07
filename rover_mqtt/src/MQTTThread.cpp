#include "MQTTThread.hpp"
#include <unistd.h>

#include <iostream>
#include <thread>
#include <mosquittopp.h>

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

MQTTThread::MQTTThread(const RobotSensorValues &sensor_values) :
		m_sensor_values(sensor_values) {
}

MQTTThread::~MQTTThread() {
	// TODO Auto-generated destructor stub
}

#define MQTT_BROKER_ADDR "127.0.0.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "rover"

void MQTTThread::operator ()(void) {
	mosqpp::lib_init();
	int ret;
    const std::string payload = "hello world";
	MqttRoverServer server;

	ret = server.connect(MQTT_BROKER_ADDR, MQTT_BROKER_PORT, 7200);
	if (ret != MOSQ_ERR_SUCCESS) {
		std::cerr << "Could not connect, ret = " << ret << std::endl;
		return;
	}

	std::cout << "Starting MQTT server" << std::endl;

	/* Start the mosquitto event loop. */
	server.loop_start();

	/* Publish our payload every second. */
	while (1) {
		server.publish(NULL, MQTT_TOPIC, payload.length(), payload.c_str(), 0,
				false);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
