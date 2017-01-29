#include "CameraThread.hpp"
#include <iostream>
#include <unistd.h>

#include <fstream>

#define MQTT_FRONT_CAMERA_TOPIC "/polarsys-rover/front-camera"

CameraThread::CameraThread(PiCam &pi_cam, MqttInterface &mqtt_interface)
: SelectLoopThread("camera-thread", 1000),
  m_pi_cam(pi_cam),
  m_mqtt_interface(mqtt_interface)
{
}

CameraThread::~CameraThread()
{
}

void CameraThread::timeout(void)
{
	std::cout << "Camera" << std::endl;

	std::vector<unsigned char> im = m_pi_cam.capture();

	std::cout << "Image is " << im.size() << " bytes long" << std::endl;

	m_mqtt_interface.publish(MQTT_FRONT_CAMERA_TOPIC, im.size(), im.data());

	static int n = 0;

	n++;

	std::cout << "Sending img " << n << std::endl;
}
