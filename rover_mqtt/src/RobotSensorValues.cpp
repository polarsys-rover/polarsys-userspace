/*
 * RobotSensorValues.cpp
 *
 *  Created on: Jul 7, 2016
 *      Author: simark
 */

#include "RobotSensorValues.hpp"

RobotSensorValues::RobotSensorValues() :
    m_data(),
    m_mutex()
{
    memset(&m_data, 0, sizeof(m_data));
}

RTIMU_DATA RobotSensorValues::getData(void) const
{

    RTIMU_DATA temp;
    {
	std::lock_guard<std::mutex> lock(m_mutex);
	temp = m_data;
    }

    return temp;
}

void RobotSensorValues::setData(const RTIMU_DATA &data)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_data = data;
}
