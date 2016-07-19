#include "PicoBorgRev.hpp"

#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>

float computePower(float t_s, float start, float end, float total_duration_s) {
    t_s /= total_duration_s / 2;

    if (t_s < 1)
	return end / 2 * t_s * t_s + start;
    t_s -= 1.0f;
    return -end / 2 * (t_s * (t_s - 2) - 1) + start;
}


int main() {
    PicoBorgRev pbr("/dev/i2c-1", 0x44);
    if (pbr.init()) {
	std::cout << "I am initialized" << std::endl;
    } else {
	std::cout << "It failed." << std::endl;
	return 1;
    }

    float power;

    for (float t_s = 0.0f; t_s < 4.0f; t_s += 0.1f) {
	float res = computePower(t_s, 0, 1, 4);
	pbr.SetMotors(res, res);
	printf("%f %f\n", t_s, res);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

    }

    for (float t_s = 0.0f; t_s < 4.0f; t_s += 0.1f) {
	float res = -computePower(t_s, 0, 1, 4) + 1.0f;
	pbr.SetMotors(res, res);
	printf("%f %f\n", t_s + 4.0f, res);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (!pbr.SetMotors(0, 0)) {
	return 1;
    }

}
