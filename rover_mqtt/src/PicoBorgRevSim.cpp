#include <PicoBorgRevSim.hpp>
#include <chrono>
#include <thread>
#include "tracepoints.h"
#include <cassert>
#include <unistd.h>

PicoBorgRevSim::PicoBorgRevSim(std::mutex &mutex):
m_mutex(mutex){
}

bool PicoBorgRevSim::init(void)
{
    return true;
}

void PicoBorgRevSim::fini(void)
{
}

bool PicoBorgRevSim::SetMotors(float power_left, float power_right) {
	tracepoint(rover_mqtt, SetMotors_begin);
	{

		std::lock_guard<std::mutex> lock(m_mutex);
		std::this_thread::sleep_for(std::chrono::milliseconds(9));
	
	}
	tracepoint(rover_mqtt, SetMotors_end);
    return 0;
}
