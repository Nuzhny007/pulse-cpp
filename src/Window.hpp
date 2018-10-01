#ifndef WINDOW_HPP
#define	WINDOW_HPP

#include <string>
#include <opencv2/core/core.hpp>
#include "Pulse.hpp"

class Window {
public:
    Window(Pulse& pulse);
    virtual ~Window();

    void update(cv::Mat& frame);

private:
    void drawTrackbarValues(cv::Mat& frame);
    void drawFps(cv::Mat& frame);

    Pulse& pulse;

    const std::string WINDOW_NAME;
    const std::string TRACKBAR_FACE_DETECTION_NAME;
    const std::string TRACKBAR_MAGNIFY_NAME;
    const std::string TRACKBAR_ALPHA_NAME;

    int trackbarFaceDetection;
    int trackbarMagnify;
    int trackbarAlpha;

    class Fps {
    public:
        int deltaFrames;
        uint64 currentFrame;
        uint64 lastFrame;
        double lastFpsTime;
        double fps;

        Fps();
        bool update();
    };

    Fps fps;
    std::string fpsString;
	cv::Point fpsPoint;

};

#endif	/* WINDOW_HPP */

