#include "PiSenseHatReal.hpp"

#include <iostream>

#include "RobotSensorValues.hpp"
#include "tracepoints.h"

PiSenseHatReal::PiSenseHatReal(std::mutex &mutex)
: m_imu_settings(),
  m_imu(RTIMU::createIMU(&m_imu_settings)),
  m_mutex(mutex)
{
}

bool PiSenseHatReal::init(void)
{
    if (!m_imu->IMUInit()) {
	std::cerr << "Could not initialize RTIMU." << std::endl;
	return false;
    }

    if (m_imu->IMUType() == RTIMU_TYPE_NULL) {
	return false;
    }

    std::cout << "Initialized IMU: " <<  m_imu->IMUName() << std::endl;

    return true;
}

void PiSenseHatReal::fini(void)
{
}

RTIMU_DATA PiSenseHatReal::ReadValues()
{
    tracepoint(rover_mqtt, ReadHat_begin);
    /* IMU */
    RTIMU_DATA imu_data;
    InitInvalidIMUData(imu_data);
	if (m_imu->IMURead()) {
		std::lock_guard<std::mutex> lock(m_mutex);
		imu_data = m_imu->getIMUData();
	}
    tracepoint(rover_mqtt, ReadHat_end);
    return imu_data;
}

PiSenseHatReal::~PiSenseHatReal() {
    fini();
}
