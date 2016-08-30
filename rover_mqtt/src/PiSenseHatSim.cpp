#include "PiSenseHatSim.hpp"

#include "RobotSensorValues.hpp"

PiSenseHatSim::PiSenseHatSim()
{
}

PiSenseHatSim::~PiSenseHatSim()
{
}

bool PiSenseHatSim::init(void)
{
    return true;
}

void PiSenseHatSim::fini(void)
{
}

RTIMU_DATA PiSenseHatSim::ReadValues(void) {
    RTIMU_DATA data;

    InitInvalidIMUData(data);

    return data;
}
