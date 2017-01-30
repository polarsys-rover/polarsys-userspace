#include "CameraThread.hpp"
#include <iostream>
#include <unistd.h>

#include <fstream>

#define MQTT_FRONT_CAMERA_TOPIC "/polarsys-rover/front-camera"

CameraThread::CameraThread(PiCam &pi_cam, MqttInterface &mqtt_interface)
: SelectLoopThread("camera-thread", 100),
  m_pi_cam(pi_cam),
  m_mqtt_interface(mqtt_interface)
{
}

CameraThread::~CameraThread()
{
}

void CameraThread::timeout(void)
{
	std::vector<unsigned char> im = m_pi_cam.capture();

	m_mqtt_interface.publish(MQTT_FRONT_CAMERA_TOPIC, im.size(), im.data());
}
