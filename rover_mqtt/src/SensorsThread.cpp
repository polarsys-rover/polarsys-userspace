/*
 * SensorsThread.cpp
 *
 *  Created on: Jul 7, 2016
 *      Author: simark
 */

#include <SensorsThread.hpp>
#include <iostream>
#include <unistd.h>

SensorsThread::SensorsThread(RobotSensorValues &sensor_values)
: SelectLoopThread("sensors-thread", 100),
  m_sensor_values(sensor_values),
  m_imu_settings(),
  m_imu(RTIMU::createIMU(&m_imu_settings))
{
    m_imu->IMUInit();
}

SensorsThread::~SensorsThread() {
}

void SensorsThread::timeout(void) {
    if (m_imu->IMURead()) {
	m_sensor_values.setData(m_imu->getIMUData());
    }
}
