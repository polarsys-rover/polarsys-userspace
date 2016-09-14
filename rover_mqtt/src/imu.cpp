/*
 * imu.c
 *
 * Test the values of rpi sensor hat
 *
 *	Created on: Jul 7, 2016
 *		Author: francis
 */

#include <iostream>
#include <cstdio>
#include <RTIMULib.h>

int main(int argc, char **argv)
{
	RTIMUSettings settings;
	RTIMU *imu = RTIMU::createIMU(&settings);
	imu->IMUInit();

	RTPressure *pressure = RTPressure::createPressure(&settings);
	pressure->pressureInit();
	printf("pressure 0x%x\n", settings.m_I2CPressureAddress);

	RTHumidity *humidity = RTHumidity::createHumidity(&settings);
	humidity->humidityInit();
	printf("humidity 0x%x\n", settings.m_I2CHumidityAddress);

	while(true) {
		imu->IMURead();
		const RTIMU_DATA data = imu->getIMUData();
		RTVector3 gyro = data.gyro;
		RTVector3 accel = data.accel;
		//gyro.normalize();
		printf("(%4.1f, %4.1f, %4.1f) (%4.1f, %4.1f, %4.1f)\r",
				gyro.x(), gyro.y(), gyro.z(),
				accel.x(), accel.y(), accel.z());
		fflush(stdout);
		sleep(0.1);
	}
	return 0;
}
