#ifndef PISENSEHAT_HPP_
#define PISENSEHAT_HPP_

#include <RTIMULib.h>

class PiSenseHat {
public:
    virtual ~PiSenseHat() {};

    virtual bool init(void) = 0;
    virtual void fini(void) = 0;

    virtual RTIMU_DATA ReadValues(void) = 0;
};

#endif /* PISENSEHAT_HPP_ */
