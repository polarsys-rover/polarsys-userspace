#ifndef CAMERA_THREAD_HPP_
#define CAMERA_THREAD_HPP_

#include "SelectLoopThread.hpp"
#include "PiCam.hpp"

#include "MqttInterface.hpp"

class CameraThread : public SelectLoopThread {
public:
	CameraThread(PiCam &pi_cam, MqttInterface &mqtt_interface);
	virtual ~CameraThread();

	virtual void timeout(void);

private:
	CameraThread(const CameraThread&) = delete;
	void operator=(const CameraThread&) = delete;

	PiCam &m_pi_cam;
	MqttInterface &m_mqtt_interface;
};

#endif /* CAMERA_THREAD_HPP_ */
