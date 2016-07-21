#ifndef PICOBORGREV_HPP_
#define PICOBORGREV_HPP_

class PicoBorgRev {
public:
    virtual ~PicoBorgRev() {}

    virtual bool init(void) = 0;
    virtual void fini(void) = 0;

    virtual bool SetMotors(float power_left, float power_right) = 0;
};

#endif /* PICOBORGREV_HPP_ */
