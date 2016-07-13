#include <iostream>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <functional>

#include <csignal>

#include <mosquittopp.h>
#include <SensorsThreadSimulated.hpp>
#include "MQTTThread.hpp"

static std::condition_variable should_quit_cv;
static std::mutex should_quit_mutex;
static bool should_quit = false;

static void sigIntHandler(int signum) {
	std::cout << "Caught signal " << signum << ", try to quit." << std::endl;

	std::unique_lock<std::mutex> should_quit_lock(should_quit_mutex);
	should_quit = true;
	should_quit_cv.notify_all();
}

int main() {
    RobotSensorValues sensor_values;

    SensorsThreadSimulated sensors_callback(sensor_values);
    std::thread sensors_thread(std::ref(sensors_callback));

    MQTTThread mqtt_callback(sensor_values);
    std::thread mqtt_thread(std::ref(mqtt_callback));

    signal(SIGINT, sigIntHandler);

    std::unique_lock<std::mutex> should_quit_lock(should_quit_mutex);
    should_quit_cv.wait(should_quit_lock, []{ return should_quit; });

    std::cout << "main thread unblocked, trying to quit." << std::endl;

    sensors_callback.please_stop();
    mqtt_callback.please_stop();

    sensors_thread.join();
    mqtt_thread.join();

    return 0;
}
