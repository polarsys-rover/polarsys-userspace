/*
 * SensorsThreadSimulated.cpp
 *
 *  Created on: Jul 7, 2016
 *      Author: simark
 */

#include "SensorsThreadSimulated.hpp"

#include <iostream>
#include <unistd.h>

SensorsThreadSimulated::SensorsThreadSimulated(RobotSensorValues &sensor_values)
: m_sensor_values(sensor_values)
{

}

SensorsThreadSimulated::~SensorsThreadSimulated() {

}

void SensorsThreadSimulated::operator ()(void) {
	while (1) {
		Accel accel = m_sensor_values.getAccel();

		accel.x += 1;
		accel.y += 2;
		accel.z += 3;

		m_sensor_values.setAccel(accel);

		sleep (1);
	}
}
