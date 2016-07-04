#include <iostream>
#include <mosquittopp.h>
#include <chrono>
#include <thread>

using namespace std;

class MqttClient : public mosqpp::mosquittopp {

public:
    MqttClient() : mosqpp::mosquittopp() {}

    void on_connect(int rc) {
        cout << "on_connect: " << rc << endl;
    }
    virtual void on_disconnect() {
        cout << "on_disconnect: " << endl;
    }
    virtual void on_message(const mosquitto_message *message) {
        cout << "on_message: " << message << endl;
    }
};

int main()
{
    cout << "mosquitto client" << endl;

    mosqpp::lib_init();

    MqttClient client1;
    MqttClient client2;

    client1.connect("localhost", 1883, 7200, "0.0.0.0");
    client2.connect("localhost", 1883, 7200, "0.0.0.0");

    client1.loop_start();
    client2.loop_start();

    client1.publish(nullptr, "rover/foo");
    client2.subscribe(nullptr, "rover/foo");

    this_thread::sleep_for(chrono::milliseconds(1000));

    client1.loop_stop();
    client2.loop_stop();

    client1.disconnect();
    client2.disconnect();

    mosqpp::lib_cleanup();

    return 0;
}

