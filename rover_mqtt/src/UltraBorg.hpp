#ifndef SRC_ULTRABORG_HPP_
#define SRC_ULTRABORG_HPP_

class UltraBorg {
public:
    virtual ~UltraBorg() {};

    virtual bool init(void) = 0;
    virtual void fini(void) = 0;

    virtual uint16_t GetDistance1(void) = 0;
};

#endif /* SRC_ULTRABORG_HPP_ */
