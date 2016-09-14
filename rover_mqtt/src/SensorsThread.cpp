/*
 * SensorsThread.cpp
 *
 *	Created on: Jul 7, 2016
 *		Author: simark
 */

#include <SensorsThread.hpp>
#include <iostream>
#include <unistd.h>

SensorsThread::SensorsThread(RobotSensorValues &sensor_values, UltraBorg &ultra_borg,
				 PiSenseHat &pi_sense_hat)
: SelectLoopThread("sensors-thread", 200),
  m_sensor_values(sensor_values),
  m_ultra_borg(ultra_borg),
  m_pi_sense_hat(pi_sense_hat)
{

}

SensorsThread::~SensorsThread() {
}

void SensorsThread::timeout(void) {
	/* IMU */
	RTIMU_DATA imu_data;

	imu_data = m_pi_sense_hat.ReadValues();

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
