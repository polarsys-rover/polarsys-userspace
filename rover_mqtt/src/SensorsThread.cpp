/*
 * SensorsThread.cpp
 *
 *  Created on: Jul 7, 2016
 *      Author: simark
 */

#include <SensorsThread.hpp>
#include <iostream>
#include <unistd.h>

SensorsThread::SensorsThread(RobotSensorValues &sensor_values, UltraBorg &ultra_borg)
: SelectLoopThread("sensors-thread", 200),
  m_sensor_values(sensor_values),
  m_imu_settings(),
  m_imu(RTIMU::createIMU(&m_imu_settings)),
  m_ultra_borg(ultra_borg)
{
    m_imu->IMUInit();
    std::cout << "Created IMU: " <<  m_imu->IMUName() << std::endl;

    /* If no hardware IMU is detected, RTIMUlib instantiates RTIMUNull, which
     * simply returns IMU data provided by us, through setIMUData.  We have to
     * set it at least once, otherwise it will return junk data. */
    if (m_imu->IMUType() == RTIMU_TYPE_NULL) {
	RTIMUNull *null_imu = dynamic_cast<RTIMUNull *>(m_imu.get());
	RTIMU_DATA imu_data;
	InitInvalidIMUData(imu_data);
	null_imu->setIMUData(imu_data);
    }
}

SensorsThread::~SensorsThread() {
}

void SensorsThread::timeout(void) {
    /* IMU */
    RTIMU_DATA imu_data;
    InitInvalidIMUData(imu_data);

    if (m_imu->IMURead()) {
	imu_data = m_imu->getIMUData();
    }

    m_sensor_values.setIMUData(imu_data);

    /* Sonar, the object is initialized to "invalid" by default. */
    SonarDistance sonar_distance;

    uint16_t sonar_distance_val = m_ultra_borg.GetDistance1();
    if (sonar_distance_val != 0 && sonar_distance_val != 0xffff) {
	sonar_distance.sonar_distance = sonar_distance_val;
	sonar_distance.sonar_distance_valid = true;
    }

    m_sensor_values.setSonarDistance(sonar_distance);
}
