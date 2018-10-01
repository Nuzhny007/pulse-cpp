#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "EvmGdownIIR.hpp"
#include "Pulse.hpp"
#include "Window.hpp"
#include "profiler/Profiler.h"

static void writeVideo(cv::VideoCapture& capture, const cv::Mat& frame);

int main(int argc, const char** argv) {
    const bool shouldWrite = false;
    const bool shouldFlip = true;

	cv::VideoCapture capture;
	if (argc < 2)
	{
		capture.open(0);
	}
	else
	{
		capture.open(argv[argc - 1]);
	}

    const int WIDTH  = capture.get(cv::CAP_PROP_FRAME_WIDTH);
    const int HEIGHT = capture.get(cv::CAP_PROP_FRAME_HEIGHT);
    const double FPS = capture.get(cv::CAP_PROP_FPS);
    std::cout << "SIZE: " << WIDTH << "x" << HEIGHT << std::endl;

    Pulse pulse;
    if (FPS != 0) {
        std::cout << "FPS: " << FPS << std::endl;
        pulse.fps = FPS;
    }
    pulse.load("res/lbpcascade_frontalface.xml");
    pulse.start(WIDTH, HEIGHT);

    Window window(pulse);

    cv::Mat frame;
    while (true) {
        PROFILE_SCOPED_DESC("loop");

        PROFILE_START_DESC("capture");
        capture >> frame;
        PROFILE_STOP();

        if (frame.empty()) {
            PROFILE_PAUSE_SCOPED(); // loop
            while (cv::waitKey() != 27) {}
            break;
        }

        PROFILE_START_DESC("flip");
        if (shouldFlip)
            flip(frame, frame, 1);
        PROFILE_STOP();

        window.update(frame);

        if (shouldWrite) {
            writeVideo(capture, frame);
        }

        PROFILE_START_DESC("wait key");
        if (cv::waitKey(1) == 27) {
            PROFILE_STOP(); // wait key
            break;
        }
        PROFILE_STOP();
    }

    Profiler::detect(argc, argv);
    Profiler::dumphtml();
    return 0;
}

static void writeVideo(cv::VideoCapture& capture, const cv::Mat& frame)
{
    static cv::VideoWriter writer("out.avi",
            cv::VideoWriter::fourcc('X', 'V', 'I', 'D'),
            capture.get(cv::CAP_PROP_FPS),
            cv::Size(capture.get(cv::CAP_PROP_FRAME_WIDTH),
            capture.get(cv::CAP_PROP_FRAME_HEIGHT)));

    writer << frame;
}
