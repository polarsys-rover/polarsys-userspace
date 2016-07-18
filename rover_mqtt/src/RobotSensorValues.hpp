#ifndef ROBOTSENSORVALUES_HPP_
#define ROBOTSENSORVALUES_HPP_

#include <cstdint>
#include <mutex>
#include <RTIMULib.h>

struct SonarDistance {
    SonarDistance()
    : sonar_distance(0),
      sonar_distance_valid(false)
    {}

    uint16_t sonar_distance;
    bool sonar_distance_valid;
};

static inline void InitInvalidIMUData(RTIMU_DATA &imu_data)
{
    imu_data.accelValid = false;
    imu_data.compassValid = false;
    imu_data.fusionPoseValid = false;
    imu_data.fusionQPoseValid = false;
    imu_data.gyroValid = false;
    imu_data.humidityValid = false;
    imu_data.pressureValid = false;
    imu_data.temperatureValid = false;
}

class RobotSensorValues {
public:
	RobotSensorValues();

	RTIMU_DATA getIMUData(void) const;
	void setIMUData(const RTIMU_DATA &data);

	SonarDistance getSonarDistance(void) const;
	void setSonarDistance(const SonarDistance &sonar_distance);

private:
	RTIMU_DATA m_imu_data;

	SonarDistance m_sonar_distance;

	mutable std::mutex m_mutex;
};

#endif /* ROBOTSENSORVALUES_HPP_ */
