#ifndef PULSE_HPP
#define	PULSE_HPP

#include <string>
#include <vector>
#include <opencv2/objdetect/objdetect.hpp>
#include "EvmGdownIIR.hpp"

class Pulse {
public:
    Pulse();
    virtual ~Pulse();

    void load(const std::string& filename);
    void start(int width, int height);
    void onFrame(cv::Mat& frame);

    int maxSignalSize;
    double relativeMinFaceSize;
    struct {
        bool enabled;
        int disabledFaceId;
    } faceDetection;
    double fps;
    struct {
        bool magnify;
        double alpha;
    } evm;

    struct Face {
        int id;
        int deleteIn;
        bool selected;

		cv::Rect box;
		cv::Mat1d timestamps;
		cv::Mat1d raw;
		cv::Mat1d pulse;
        int noPulseIn;
        bool existsPulse;

		cv::Mat1d bpms;
        double bpm;

        struct {
            EvmGdownIIR evm;
            cv::Mat out;
			cv::Rect box;
        } evm;

        struct Peaks {
			cv::Mat1i indices;
			cv::Mat1d timestamps;
			cv::Mat1d values;

            void push(int index, double timestamp, double value);
            void pop();
            void clear();
        } peaks;

        Face(int id, const cv::Rect& box, int deleteIn);
        int nearestBox(const std::vector<cv::Rect>& boxes);
        void updateBox(const cv::Rect& box);
        void reset();
    };

	std::vector<Face> faces;

private:
    int nearestFace(const cv::Rect& box);
    void onFace(cv::Mat& frame, Face& face, const cv::Rect& box);
    void peaks(Face& face);
    void bpm(Face& face);
    void draw(cv::Mat& frame, const Face& face, const cv::Rect& box);

    double now;
    double lastFaceDetectionTimestamp;
    double lastBpmTimestamp;
	cv::Size minFaceSize;
	cv::CascadeClassifier classifier;
	cv::Mat gray;
	std::vector<cv::Rect> boxes;
	cv::Mat1d powerSpectrum;
    int nextFaceId;
    int deleteFaceIn;
    int holdPulseFor;
    double currentFps;

};

#endif	/* PULSE_HPP */

