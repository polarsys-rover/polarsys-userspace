#ifndef SRC_MOTORSCONTROLTHREAD_HPP_
#define SRC_MOTORSCONTROLTHREAD_HPP_

#include "SelectLoopThread.hpp"

#include "MqttInterface.hpp"
#include <mutex>

#include "controls.pb.h"
#include "PicoBorgRevReal.hpp"

class MotorsControlThread: public SelectLoopThread {
public:
    MotorsControlThread(PicoBorgRev &pico_borg_rev, MqttInterface &mqtt_interface);
    virtual ~MotorsControlThread();

    void on_message(std::string payload);
    void timeout(void);

private:
    PicoBorgRev &m_pico_borg_rev;
    MqttInterface &m_mqtt_interface;

    float m_target_power_left;
    float m_target_power_right;

    std::mutex m_mutex;

    PolarsysRover::RoverControls m_protobuf_controls;

    MotorsControlThread(const MotorsControlThread &) = delete;
    void operator=(const MotorsControlThread &) = delete;
};

#endif /* SRC_MOTORSCONTROLTHREAD_HPP_ */
