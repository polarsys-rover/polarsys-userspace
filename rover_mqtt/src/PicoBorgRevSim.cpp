#include <PicoBorgRevSim.hpp>

#include "tracepoints.h"

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
	tracepoint(rover_mqtt, SetMotors_end);
    return 0;
}
