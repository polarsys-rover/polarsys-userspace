#include <iostream>
#include <chrono>
#include <thread>

#include <mosquittopp.h>

class MqttRoverServer : public mosqpp::mosquittopp {
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

#define MQTT_BROKER_ADDR "127.0.0.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "rover"

int main()
{
	int ret;
    const std::string payload = "hello world";

    std::cout << "Starting MQTT server" << std::endl;

    mosqpp::lib_init();

    MqttRoverServer server;
    ret = server.connect(MQTT_BROKER_ADDR, MQTT_BROKER_PORT, 7200);
    if (ret != MOSQ_ERR_SUCCESS) {
    	std::cerr << "Could not connect, ret = " << ret << std::endl;
    	goto connect_failed;
    }

    /* Start the mosquitto event loop. */
    server.loop_start();

    /* Publish our payload every second. */
    while (1) {
    	server.publish(NULL, MQTT_TOPIC, payload.length(), payload.c_str(), 0, false);
    	std::this_thread::sleep_for(std::chrono::seconds(1));
    }

connect_failed:
    mosqpp::lib_cleanup();

    return 0;
}
