/*
 * SensorsThreadSimulated.cpp
 *
 *  Created on: Jul 7, 2016
 *      Author: simark
 */

#include <SensorsThreadSimulated.hpp>
#include <iostream>
#include <unistd.h>

SensorsThreadSimulated::SensorsThreadSimulated(RobotSensorValues &sensor_values)
: SelectLoopThread("sensors-thread", 1000), m_sensor_values(sensor_values)
{

}

SensorsThreadSimulated::~SensorsThreadSimulated() {

}

void SensorsThreadSimulated::timeout(void) {
    Accel accel = m_sensor_values.getAccel();

    accel.x += 1;
    accel.y += 2;
    accel.z += 3;

    m_sensor_values.setAccel(accel);
}
