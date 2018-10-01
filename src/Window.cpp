#include "Window.hpp"
#include <sstream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "ext/opencv.hpp"
#include "Pulse.hpp"
#include "profiler/Profiler.h"

Window::Window(Pulse& pulse) :
    pulse(pulse),
    WINDOW_NAME("EVM"),
    TRACKBAR_FACE_DETECTION_NAME("Face Detection"),
    TRACKBAR_MAGNIFY_NAME       ("Magnify       "),
    TRACKBAR_ALPHA_NAME         ("Amplification ")
{
    trackbarFaceDetection = pulse.faceDetection.enabled;
    trackbarMagnify = pulse.evm.magnify;
    trackbarAlpha = pulse.evm.alpha;

    cv::namedWindow(WINDOW_NAME.c_str(), cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO);
	cv::createTrackbar(TRACKBAR_FACE_DETECTION_NAME, WINDOW_NAME, &trackbarFaceDetection, 1);
	cv::createTrackbar(TRACKBAR_MAGNIFY_NAME, WINDOW_NAME, &trackbarMagnify, 1);
	cv::createTrackbar(TRACKBAR_ALPHA_NAME, WINDOW_NAME, &trackbarAlpha, 500);

    fpsPoint = cv::Point(10, 15);
}

Window::~Window() {
}

void Window::update(cv::Mat& frame) {
    PROFILE_SCOPED();

    // update pulse values for Eulerian video magnification
    pulse.faceDetection.enabled = trackbarFaceDetection == 1;
    pulse.evm.magnify = trackbarMagnify == 1;
    pulse.evm.alpha = trackbarAlpha;

    PROFILE_START_DESC("bgr2rgb");
    cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    PROFILE_STOP();

    // process frame
    pulse.onFrame(frame);

    drawTrackbarValues(frame);
    drawFps(frame);

    PROFILE_START_DESC("rgb2bgr");
	cv::cvtColor(frame, frame, cv::COLOR_RGB2BGR);
    PROFILE_STOP();

    PROFILE_START_DESC("imshow");
	cv::imshow(WINDOW_NAME, frame);
    PROFILE_STOP();
}

void Window::drawTrackbarValues(cv::Mat& frame) {
    PROFILE_SCOPED();

    const int namesX = 10;
    const int valuesX = 150;
    const int spaceY = 15;

    cv::putText(frame, TRACKBAR_FACE_DETECTION_NAME,                cv::Point( namesX, spaceY * 2), cv::FONT_HERSHEY_PLAIN, 1, cv::BLUE);
    cv::putText(frame, (trackbarFaceDetection == 1 ? "ON" : "OFF"), cv::Point(valuesX, spaceY * 2), cv::FONT_HERSHEY_PLAIN, 1, cv::BLUE);

	cv::putText(frame, TRACKBAR_MAGNIFY_NAME, cv::Point(namesX, spaceY * 3), cv::FONT_HERSHEY_PLAIN, 1, cv::BLUE);
	cv::putText(frame, (trackbarMagnify == 1 ? "ON" : "OFF"), cv::Point(valuesX, spaceY * 3), cv::FONT_HERSHEY_PLAIN, 1, cv::BLUE);

	cv::putText(frame, TRACKBAR_ALPHA_NAME, cv::Point( namesX, spaceY * 4), cv::FONT_HERSHEY_PLAIN, 1, cv::BLUE);
	cv::putText(frame, std::to_string(trackbarAlpha), cv::Point(valuesX, spaceY * 4), cv::FONT_HERSHEY_PLAIN, 1, cv::BLUE);
}

void Window::drawFps(cv::Mat& frame) {
    PROFILE_SCOPED();

    if (fps.update()) {
        PROFILE_SCOPED_DESC("fps string");
        std::stringstream ss;
        ss.precision(3);
        ss << "FPS: " << fps.fps;
        fpsString = ss.str();
    }

    PROFILE_START_DESC("fps drawing");
    cv::putText(frame, fpsString, fpsPoint, cv::FONT_HERSHEY_PLAIN, 1, cv::BLUE);
    PROFILE_STOP();
}

Window::Fps::Fps() {
    deltaFrames = 30;
    currentFrame = 0;
    lastFrame = 0;
    lastFpsTime = 0;
    fps = 0;
}

bool Window::Fps::update() {
    PROFILE_SCOPED();

    if (currentFrame == 0) {
        lastFpsTime = (double)cv::getTickCount();
    }

    currentFrame++;

    if (currentFrame % 30 == 0) {
        PROFILE_SCOPED_DESC("fps tick");
        double now = (double)cv::getTickCount();
        double diff = (now - lastFpsTime) * 1000. / cv::getTickFrequency();

        if (diff > 0) {
            fps = (currentFrame - lastFrame) * 1000 / diff;
            lastFrame = currentFrame;
            lastFpsTime = now;
            return true;
        }
    }

    return false;
}
