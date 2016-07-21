#include <iostream>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <functional>
#include <memory>

#include <csignal>

#include <mosquittopp.h>
#include <MotorsControlThread.hpp>
#include <SensorsPublishThread.hpp>
#include <SensorsThreadSimulated.hpp>
#include <SensorsThread.hpp>
#include "MqttInterface.hpp"
#include "controls.pb.h"

#include "PicoBorgRevReal.hpp"
#include "PicoBorgRevSim.hpp"

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
#define I2C_DEV "/dev/i2c-1"
#define I2C_ULTRA_BORG_ADDR 0x36
#define I2C_PICO_BORG_REF_ADDR 0x44

// FIXME: This is just a proof of concept, it should obviously not be there.
/*void i_got_a_message(std::string payload) {
    std::cout << "I got a message of length " << payload.size()  << " " << payload.length() << std::endl;
    PolarsysRover::RoverControls controls;
    controls.ParseFromString(payload);

    std::cout << "Message = " << controls.DebugString() << std::endl;

}*/

static const int simulate_ultra_borg = 0;
static const int simulate_pico_borg_rev = 1;

int main(int argc, char *argv[])
{
    std::unique_ptr<UltraBorg> ultra_borg_p;
    std::unique_ptr<PicoBorgRev> pico_borg_rev_p;

    mosqpp::lib_init();

    MqttInterface mqtt_interface(MQTT_BROKER_HOST, MQTT_BROKER_PORT);
    mqtt_interface.start();
    //mqtt_interface.subscribe("/polarsys-rover/controls", i_got_a_message);

    if (simulate_ultra_borg) {
	// TODO
    } else {
	ultra_borg_p.reset(new UltraBorgReal(I2C_DEV, I2C_ULTRA_BORG_ADDR));
    }

    if (simulate_pico_borg_rev) {
	pico_borg_rev_p.reset(new PicoBorgRevSim());
    } else {
	pico_borg_rev_p.reset(new PicoBorgRevReal(I2C_DEV, I2C_PICO_BORG_REF_ADDR));
    }

    if (!ultra_borg_p->init()) {
	std::cerr << "Could not initialize UltraBorg" << std::endl;
	return 1;
    }

    if (!pico_borg_rev_p->init()) {
	std::cerr << "Could not initialize PicoBorgRev" << std::endl;
	return 1;
    }

    /* Store for the most recently acquired sensor values. */
    RobotSensorValues sensor_values;

    /* Thread continuously reading the sensor values. */
    SensorsThread sensors_callback(sensor_values, *ultra_borg_p);
    std::thread sensors_thread(std::ref(sensors_callback));

    /* Thread responsible for publishing sensor values periodically. */
    SensorsPublishThread mqtt_callback(sensor_values, mqtt_interface);
    std::thread mqtt_thread(std::ref(mqtt_callback));

    /* Thread controlling the motors. */
    MotorsControlThread motors_control_callback(*pico_borg_rev_p, mqtt_interface);
    std::thread motors_control_thread(std::ref(motors_control_callback));

    /* Install ctrl-C signal handler. */
    signal(SIGINT, sigIntHandler);

    /* Wait until something tells us to quit. */
    std::unique_lock<std::mutex> should_quit_lock(should_quit_mutex);
    should_quit_cv.wait(should_quit_lock, []{ return should_quit; });

    std::cout << "main thread unblocked, trying to quit." << std::endl;

    sensors_callback.please_stop();
    mqtt_callback.please_stop();

    sensors_thread.join();
    mqtt_thread.join();

    mqtt_interface.stop();

    mosqpp::lib_cleanup();

    return 0;
}
