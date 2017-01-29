#include "PiCamReal.hpp"

#include <iostream>

PiCamReal::PiCamReal()
: PiCam(),
  m_cam()
{

}

PiCamReal::~PiCamReal()
{

}

bool PiCamReal::init()
{
	m_cam.set(CV_CAP_PROP_FORMAT, CV_8UC1);

	m_cam.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	m_cam.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	if (!m_cam.open())
		return false;

	return true;
}

void PiCamReal::fini()
{

}


std::vector<unsigned char> PiCamReal::capture()
{
	std::vector<uchar> buf;

	cv::Mat im;

	if (!m_cam.grab()) {
		std::cerr << "Error grabbing image" << std::endl;
		return std::vector<uchar>();
	}

	m_cam.retrieve(im);

	if (!cv::imencode(".jpg", im, buf)) {
		std::cerr << "Error encoding image" << std::endl;
		return std::vector<uchar>();
	}

	return std::move(buf);
}
