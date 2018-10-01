#ifndef EVMGDOWNIIR_HPP
#define	EVMGDOWNIIR_HPP

#include <opencv2/core/core.hpp>

class EvmGdownIIR {
public:
    EvmGdownIIR();
    virtual ~EvmGdownIIR();

    void onFrame(const cv::Mat& src, cv::Mat& out);

    bool first;
	cv::Size blurredSize;
    double fHigh;
    double fLow;
    int alpha;

private:
	cv::Mat srcFloat;
	cv::Mat blurred;
	cv::Mat lowpassHigh;
	cv::Mat lowpassLow;
	cv::Mat outFloat;

};

#endif	/* EVMGDOWNIIR_HPP */

