#include "PiCamReal.hpp"

#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

PiCamReal::PiCamReal()
: PiCam(),
  m_cam(),
  m_brightness(50)
{

}

PiCamReal::~PiCamReal()
{

}

bool PiCamReal::init()
{
	m_cam.set(CV_CAP_PROP_FORMAT, CV_8UC3);

	m_cam.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	m_cam.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	m_cam.set(CV_CAP_PROP_CONTRAST, 70);


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


	int sum = 0;

	int length = im.rows * im.cols * 3;

	for (int i = 0; i < length; i += 3) {
		uchar b = im.data[i];
		uchar g = im.data[i + 1];
		uchar r = im.data[i + 2];

		im.data[i] = r;
		im.data[i + 2] = b;

		sum += b + g + r;
	}

	sum /= length;

	if (sum > (128 + 20)) {
		m_brightness = std::max(0, m_brightness - 1);
		m_cam.set(CV_CAP_PROP_BRIGHTNESS, m_brightness);
	} else if (sum < (128 - 20)) {
		m_brightness = std::min(100, m_brightness + 1);
		m_cam.set(CV_CAP_PROP_BRIGHTNESS, m_brightness);
	}

	if (!cv::imencode(".jpg", im, buf)) {
		std::cerr << "Error encoding image" << std::endl;
		return std::vector<uchar>();
	}

	return std::move(buf);
}
