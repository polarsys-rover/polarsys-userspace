#include <iostream>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <functional>
#include <memory>

#include <csignal>

#include <mosquittopp.h>
#include <SensorsPublishThread.hpp>
#include <SensorsThreadSimulated.hpp>
#include <SensorsThread.hpp>
#include "MqttInterface.hpp"

static std::condition_variable should_quit_cv;
static std::mutex should_quit_mutex;
static bool should_quit = false;

static void sigIntHandler(int signum)
{
    std::cout << "Caught signal " << signum << ", try to quit." << std::endl;

    std::unique_lock<std::mutex> should_quit_lock(should_quit_mutex);
    should_quit = true;
    should_quit_cv.notify_all();
}

#define MQTT_BROKER_HOST "127.0.0.1"
#define MQTT_BROKER_PORT 1883

int main(int argc, char *argv[])
{
    mosqpp::lib_init();

    MqttInterface mqtt_interface(MQTT_BROKER_HOST, MQTT_BROKER_PORT);
    mqtt_interface.start();

    /* Store for the most recently acquired sensor values. */
    RobotSensorValues sensor_values;

    std::unique_ptr<SelectLoopThread> sensors_callback = nullptr;
    int simulated = 0;
    if (simulated) {
	/* Thread responsible for faking read sensor values. */
	sensors_callback.reset(new SensorsThreadSimulated(sensor_values));
    } else {
	sensors_callback.reset(new SensorsThread(sensor_values));
    }
    std::thread sensors_thread(std::ref(*sensors_callback));

    /* Thread responsible for publishing sensor values every second. */
    SensorsPublishThread mqtt_callback(sensor_values, mqtt_interface);
    std::thread mqtt_thread(std::ref(mqtt_callback));

    /* Install ctrl-C signal handler. */
    signal(SIGINT, sigIntHandler);

    /* Wait until something tells us to quit. */
    std::unique_lock<std::mutex> should_quit_lock(should_quit_mutex);
    should_quit_cv.wait(should_quit_lock, []{ return should_quit; });

    std::cout << "main thread unblocked, trying to quit." << std::endl;

    sensors_callback->please_stop();
    mqtt_callback.please_stop();

    sensors_thread.join();
    mqtt_thread.join();

    mqtt_interface.stop();

    mosqpp::lib_cleanup();

    return 0;
}
