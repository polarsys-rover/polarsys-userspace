#ifndef MQTTINTERFACE_HPP_
#define MQTTINTERFACE_HPP_

#include <string>
#include <functional>
#include <mosquittopp.h>
#include <map>
#include <mutex>

class MqttInterface : private mosqpp::mosquittopp {
public:
	MqttInterface(std::string broker_host, int broker_port);
	virtual ~MqttInterface();

	void start();
	void stop();

	void publish(std::string topic, int payload_len, const void *payload);

	typedef std::function<void(std::string payload)> CallbackType;
	void subscribe(std::string topic, CallbackType callback);

private:
	virtual void on_connect(int rc);
	virtual void on_disconnect(int rc);
	virtual void on_message(const mosquitto_message *message);

	void do_subscribe(const std::string &topic);

	std::string m_broker_host;
	int m_broker_port;

	std::map<std::string, CallbackType> m_subscriptions;
	std::mutex m_mutex;

	MqttInterface(const MqttInterface &) = delete;
	void operator=(const MqttInterface &) = delete;
};

#endif /* MQTTINTERFACE_HPP_ */
