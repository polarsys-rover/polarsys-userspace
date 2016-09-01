#include "PiSenseHatSim.hpp"

#include "RobotSensorValues.hpp"

static uint64_t timespec_to_ms(const struct timespec &ts) {
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

PiSenseHatSim::PiSenseHatSim()
: m_start_time(0)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    m_start_time = timespec_to_ms(ts);
}

PiSenseHatSim::~PiSenseHatSim()
{
}

bool PiSenseHatSim::init(void)
{
    return true;
}

void PiSenseHatSim::fini(void)
{
}
#include <iostream>

RTIMU_DATA PiSenseHatSim::ReadValues(void) {
    struct timespec ts;
    RTIMU_DATA imu_data;

    InitInvalidIMUData(imu_data);

    clock_gettime(CLOCK_MONOTONIC, &ts);

    uint64_t elapsed_time = timespec_to_ms(ts) - m_start_time;

    imu_data.accel.setX(20 * sin(2 * M_PI * (elapsed_time / 20000.0)));
    imu_data.accel.setY(5 * sin(2 * M_PI * (elapsed_time / 15000.0)));
    imu_data.accel.setZ(10 * sin(2 * M_PI * (elapsed_time / 10000.0)));

    imu_data.accelValid = true;

    imu_data.gyro.setX(14 * sin(2 * M_PI * (elapsed_time / 19000.0)));
    imu_data.gyro.setY(25 * sin(2 * M_PI * (elapsed_time / 22000.0)));
    imu_data.gyro.setZ(14 * sin(2 * M_PI * (elapsed_time / 17000.0)));

    imu_data.gyroValid = true;

    imu_data.compass.setX(14 * sin(2 * M_PI * (elapsed_time / 15000.0)));
    imu_data.compass.setY(1 * sin(2 * M_PI * (elapsed_time / 8000.0)));
    imu_data.compass.setZ(11 * sin(2 * M_PI * (elapsed_time / 6000.0)));

    imu_data.compassValid = true;

    return imu_data;
}
