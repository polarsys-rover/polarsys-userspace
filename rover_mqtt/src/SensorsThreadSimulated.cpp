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

void SensorsThreadSimulated::timeout(void)
{
    RTIMU_DATA data = m_sensor_values.getData();

    data.accelValid = true;
    data.accel.setX(data.accel.x() + 1);
    data.accel.setY(data.accel.y() + 2);
    data.accel.setZ(data.accel.z() + 3);

    m_sensor_values.setData(data);
}
