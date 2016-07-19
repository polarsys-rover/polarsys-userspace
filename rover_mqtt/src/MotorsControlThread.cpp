#include "MotorsControlThread.hpp"

#include <functional>
#include <iostream>

MotorsControlThread::MotorsControlThread(PicoBorgRev &pico_borg_rev, MqttInterface &mqtt_interface)
: SelectLoopThread("motors-thread", 50),
  m_pico_borg_rev(pico_borg_rev),
  m_mqtt_interface(mqtt_interface),
  m_target_power_left(0),
  m_target_power_right(0),
  m_mutex(),
  m_protobuf_controls()
{
    m_mqtt_interface.subscribe("/polarsys-rover/controls",
	    std::bind(&MotorsControlThread::on_message, this, std::placeholders::_1));
}

MotorsControlThread::~MotorsControlThread()
{
}

void MotorsControlThread::on_message(std::string payload)
{
    if (!m_protobuf_controls.ParseFromString(payload)) {
	std::cerr << "MotorsControlThread::on_message: failed to parse protobuf." << std::endl;
    }

    std::lock_guard<std::mutex> lock(m_mutex);

    m_target_power_left = m_protobuf_controls.left() / 100.0f;
    m_target_power_right = m_protobuf_controls.right() / 100.0f;
}

void MotorsControlThread::timeout(void)
{
    m_pico_borg_rev.SetMotors(m_target_power_left, m_target_power_right);
}
