#ifndef ROBOTSENSORVALUES_HPP_
#define ROBOTSENSORVALUES_HPP_

#include <cstdint>
#include <mutex>

class Accel {
public:
	Accel()
	: Accel(0, 0, 0)
	{}

	Accel(int32_t x, int32_t y, int32_t z)
	: x(x), y(y), z(z)
	{}

	int32_t x, y, z;
};

class RobotSensorValues {
public:
	RobotSensorValues();
	virtual ~RobotSensorValues();

	void setAccel(Accel accel);
	Accel getAccel(void) const;

private:
	Accel m_accel;

	mutable std::mutex m_mutex;
};

#endif /* ROBOTSENSORVALUES_HPP_ */
