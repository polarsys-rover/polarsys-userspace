#include "UltraBorgSim.hpp"

static uint64_t timespec_to_ms(const struct timespec &ts) {
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

UltraBorgSim::UltraBorgSim()
: m_start_time(0)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    m_start_time = timespec_to_ms(ts);
}

UltraBorgSim::~UltraBorgSim()
{
}

bool UltraBorgSim::init(void)
{
    return true;
}

void UltraBorgSim::fini(void)
{
}

uint16_t UltraBorgSim::GetDistance1(void) {
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);



    uint64_t elapsed_time = timespec_to_ms(ts) - m_start_time;
    uint64_t x = elapsed_time % 10000;
    uint64_t b = 500;
    double m = 15000.0 / 10000.0;

    return m * x + b;
}
