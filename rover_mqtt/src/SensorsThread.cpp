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
				 PiSenseHat &pi_sense_hat, ADS1115 &ads1115)
: SelectLoopThread("sensors-thread", 200),
  m_sensor_values(sensor_values),
  m_ultra_borg(ultra_borg),
  m_pi_sense_hat(pi_sense_hat),
  m_ads1115(ads1115)
{

}

SensorsThread::~SensorsThread() {
}

void SensorsThread::timeout(void) {
	/* Start the conversion first, so it converts while we read other
	 * sensors. */
	bool ads1115_conversion_started = false;

	if (m_ads1115.initialized()) {
		ads1115_conversion_started = m_ads1115.StartConversion();
	}

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

	/* The BatteryVoltage object is initialized to "invalid" by default. */
	BatteryVoltage bv;
	if (ads1115_conversion_started) {
		uint16_t voltage;
		if (m_ads1115.WaitAndGetConversionResult(&voltage)) {
			bv.valid = true;
			bv.value = voltage;
		}
	}

	m_sensor_values.setBatteryVoltage(bv);
}
