#include <iostream>
#include <chrono>
#include <thread>

#include <mosquittopp.h>

#include "SensorsThreadSimulated.hpp"
#include "MQTTThread.hpp"

int main()
{

    RobotSensorValues sensor_values;

    SensorsThreadSimulated sensors_callback(sensor_values);
    std::thread sensors_thread(sensors_callback);

    MQTTThread mqtt_callback(sensor_values);
    std::thread mqtt_thread(mqtt_callback);


    sensors_thread.join();
    mqtt_thread.join();

    return 0;
}
