#ifndef DETECTOR_H
#define DETECTOR_H

#include <string>

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"


class Detector
{
private:

public:
    cv::CascadeClassifier createClassifier(const std::string xmlFilepath);
    cv::Mat detectClassifier(const cv::CascadeClassifier classifier, const cv::Mat original_frame);
    cv::Mat detectLines(const cv::Mat original_frame);
    cv::Mat cannyFilter(const cv::Mat original_frame);
};

#endif