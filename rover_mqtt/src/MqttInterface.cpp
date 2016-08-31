#include "MqttInterface.hpp"

#include <cassert>
#include <iostream>

MqttInterface::MqttInterface(std::string broker_host, int broker_port)
: m_broker_host(broker_host),
  m_broker_port(broker_port),
  m_subscriptions()
{
}

MqttInterface::~MqttInterface()
{
}

void MqttInterface::start(void)
{
    mosqpp::mosquittopp::reconnect_delay_set(3, 30, true);
    mosqpp::mosquittopp::connect_async(m_broker_host.c_str(), m_broker_port);

    std::cout << "Starting MQTT server" << std::endl;

    mosqpp::mosquittopp::loop_start();
}

void MqttInterface::stop()
{
    mosqpp::mosquittopp::loop_stop(true);
}

void MqttInterface::publish(std::string topic, int payload_len, const void *payload)
{
    mosqpp::mosquittopp::publish(NULL, topic.c_str(), payload_len, payload, 0, false);
}


void MqttInterface::on_connect(int rc)
{
    std::cout << "MQTT connected!" << std::endl;
}

void MqttInterface::on_disconnect(int rc)
{
    std::cout << "MQTT disconnected!" << std::endl;
}

void MqttInterface::on_message(const mosquitto_message *message)
{
    std::string s((const char *) message->payload, message->payloadlen);

    for (auto& pair : m_subscriptions) {
	bool res;
	std::string sub = pair.first;
	CallbackType &cb = pair.second;

	if (mosqpp::topic_matches_sub(sub.c_str(), message->topic, &res) == MOSQ_ERR_SUCCESS) {
	    if (res) {
		cb(s);
	    }
	}
    }
}

void MqttInterface::subscribe(std::string topic, CallbackType callback)
{
    mosqpp::mosquittopp::subscribe(NULL, topic.c_str(), 0);
    m_subscriptions[topic] = callback;
}
