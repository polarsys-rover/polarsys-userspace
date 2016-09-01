#include <iostream>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <functional>
#include <memory>

#include <csignal>

#include <boost/program_options.hpp>

#include <mosquittopp.h>
#include <MotorsControlThread.hpp>
#include <SensorsPublishThread.hpp>
#include <SensorsThread.hpp>
#include "MqttInterface.hpp"
#include "controls.pb.h"

#include "PicoBorgRevReal.hpp"
#include "PicoBorgRevSim.hpp"

#include "UltraBorgReal.hpp"
#include "UltraBorgSim.hpp"

#include "PiSenseHatReal.hpp"
#include "PiSenseHatSim.hpp"

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

struct options {
    options()
    : simulate_ultra_borg(0),
      simulate_pico_borg_rev(0),
      simulate_pi_sense_hat(0)
    {
    }

    void do_stuff(int a, char c, std::string b) {

    }

    int simulate_ultra_borg;
    int simulate_pico_borg_rev;
    int simulate_pi_sense_hat;
};

static int parse_args(int argc, char *argv[], options &opts) {
    namespace po = boost::program_options;

    po::options_description desc("rover_mqtt");
    desc.add_options()
	    ("help", "Show this help")
	    ("simulate-ultra-borg", "Simulate the UltraBorg module")
	    ("simulate-pico-borg-rev", "Simulate the PicoBorgRev module")
	    ("simulate-pi-sense-hat", "Simulate the Raspberry Pi Sense Hat module");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
	std::cout << desc << std::endl;
	return 1;
    }

    if (vm.count("simulate-ultra-borg")) {
	opts.simulate_ultra_borg = 1;
    }

    if (vm.count("simulate-pico-borg-rev")) {
	opts.simulate_pico_borg_rev = 1;
    }

    if (vm.count("simulate-pi-sense-hat")) {
	opts.simulate_pi_sense_hat = 1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    std::unique_ptr<UltraBorg> ultra_borg_p;
    std::unique_ptr<PicoBorgRev> pico_borg_rev_p;
    std::unique_ptr<PiSenseHat> pi_sense_hat_p;

    options opts;

    try {
	if (parse_args(argc, argv, opts)) {
	    return 0;
	}
    } catch (boost::program_options::unknown_option &ex) {
	std::cerr << ex.what() << std::endl;
	return 1;
    }

    mosqpp::lib_init();

    MqttInterface mqtt_interface(MQTT_BROKER_HOST, MQTT_BROKER_PORT);
    mqtt_interface.start();

    if (opts.simulate_ultra_borg) {
	ultra_borg_p.reset(new UltraBorgSim());
    } else {
	ultra_borg_p.reset(new UltraBorgReal(I2C_DEV, I2C_ULTRA_BORG_ADDR));
    }

    if (opts.simulate_pico_borg_rev) {
	pico_borg_rev_p.reset(new PicoBorgRevSim());
    } else {
	pico_borg_rev_p.reset(new PicoBorgRevReal(I2C_DEV, I2C_PICO_BORG_REF_ADDR));
    }

    if (opts.simulate_pi_sense_hat) {
	pi_sense_hat_p.reset(new PiSenseHatSim());
    } else {
	pi_sense_hat_p.reset(new PiSenseHatReal());
    }

    if (!ultra_borg_p->init()) {
	std::cerr << "Could not initialize UltraBorg" << std::endl;
	return 1;
    }

    if (!pico_borg_rev_p->init()) {
	std::cerr << "Could not initialize PicoBorgRev" << std::endl;
	return 1;
    }

    if (!pi_sense_hat_p->init()) {
	std::cerr << "Could not initialize PiSenseHat" << std::endl;
	return 1;
    }

    /* Store for the most recently acquired sensor values. */
    RobotSensorValues sensor_values;

    /* Thread continuously reading the sensor values. */
    SensorsThread sensors_callback(sensor_values, *ultra_borg_p, *pi_sense_hat_p);
    std::thread sensors_thread(std::ref(sensors_callback));
    pthread_setname_np(sensors_thread.native_handle(),
		       sensors_callback.getName().c_str());

    /* Thread responsible for publishing sensor values periodically. */
    SensorsPublishThread mqtt_callback(sensor_values, mqtt_interface);
    std::thread mqtt_thread(std::ref(mqtt_callback));
    pthread_setname_np(mqtt_thread.native_handle(),
		       mqtt_callback.getName().c_str());

    /* Thread controlling the motors. */
    MotorsControlThread motors_control_callback(*pico_borg_rev_p, mqtt_interface);
    std::thread motors_control_thread(std::ref(motors_control_callback));
    pthread_setname_np(motors_control_thread.native_handle(),
		       motors_control_callback.getName().c_str());

    /* Install ctrl-C signal handler. */
    signal(SIGINT, sigIntHandler);

    /* Wait until something tells us to quit. */
    std::unique_lock<std::mutex> should_quit_lock(should_quit_mutex);
    should_quit_cv.wait(should_quit_lock, []{ return should_quit; });

    std::cout << "main thread unblocked, trying to quit." << std::endl;

    sensors_callback.please_stop();
    mqtt_callback.please_stop();
    motors_control_callback.please_stop();

    sensors_thread.join();
    mqtt_thread.join();
    motors_control_thread.join();

    mqtt_interface.stop();

    mosqpp::lib_cleanup();

    return 0;
}
