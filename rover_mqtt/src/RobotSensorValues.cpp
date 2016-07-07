/*
 * RobotSensorValues.cpp
 *
 *  Created on: Jul 7, 2016
 *      Author: simark
 */

#include "RobotSensorValues.hpp"

RobotSensorValues::RobotSensorValues() {
	// TODO Auto-generated constructor stub

}

RobotSensorValues::~RobotSensorValues() {
	// TODO Auto-generated destructor stub
}

void RobotSensorValues::setAccel(Accel accel) {

}

Accel RobotSensorValues::getAccel(void) const {
	Accel ret;

	{
		std::lock_guard<std::mutex> lock(m_mutex);
		ret = m_accel;
	}

	return ret;
}
