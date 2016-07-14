#ifndef MQTTINTERFACE_HPP_
#define MQTTINTERFACE_HPP_

#include <string>
#include <functional>
#include <mosquittopp.h>
#include <map>

class MqttInterface : private mosqpp::mosquittopp {
public:
    MqttInterface(std::string broker_host, int broker_port);

    void start();
    void stop();

    void publish(std::string topic, int payload_len, const void *payload);

    typedef std::function<void()> CallbackType;
    void subscribe(std::string topic, CallbackType callback);

private:
    virtual void on_connect(int rc);
    virtual void on_disconnect();
    virtual void on_message(const mosquitto_message *message);

    std::string m_broker_host;
    int m_broker_port;

    std::map<std::string, CallbackType> m_subscriptions;

    MqttInterface(const MqttInterface &) = delete;
    void operator=(const MqttInterface &) = delete;
};

#endif /* MQTTINTERFACE_HPP_ */
