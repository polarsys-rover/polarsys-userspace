/*
 * RobotSensorValues.cpp
 *
 *	Created on: Jul 7, 2016
 *		Author: simark
 */

#include "RobotSensorValues.hpp"

RobotSensorValues::RobotSensorValues() :
	m_imu_data(),
	m_sonar_distance(),
	m_mutex()
{
	InitInvalidIMUData(m_imu_data);
}

RTIMU_DATA RobotSensorValues::getIMUData(void) const
{

	RTIMU_DATA temp;

	{
	std::lock_guard<std::mutex> lock(m_mutex);
	temp = m_imu_data;
	}

	return temp;
}

void RobotSensorValues::setIMUData(const RTIMU_DATA &data)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_imu_data = data;
}

SonarDistance RobotSensorValues::getSonarDistance(void) const
{
	SonarDistance temp;

	{
	std::lock_guard<std::mutex> lock(m_mutex);
	temp = m_sonar_distance;
	}

	return temp;
}

void RobotSensorValues::setSonarDistance(const SonarDistance &sonar_distance)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_sonar_distance = sonar_distance;
}
