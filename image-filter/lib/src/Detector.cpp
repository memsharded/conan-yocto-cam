#include <fmt/format.h>

#include "Detector.h"


cv::CascadeClassifier Detector::createClassifier(const std::string xmlFilepath)
{
    cv::CascadeClassifier classifier;
    if (!classifier.load(xmlFilepath))
    {
        std::cout << fmt::format("Error loading file: {}", xmlFilepath) << std::endl;
        throw std::runtime_error{fmt::format("Error loading file: {}", xmlFilepath)};
    }
    return classifier;
}

cv::Mat Detector::detectClassifier(cv::CascadeClassifier classifier, const cv::Mat originalFrame)
{
    cv::Mat frame = originalFrame.clone();
    std::vector<cv::Rect> items;
    cv::Mat frame_gray;

    cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(frame_gray, frame_gray);

    //-- Detect

    classifier.detectMultiScale(frame_gray, items, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

    for(size_t i = 0; i < items.size(); i++)
    {
        cv::Point center(items[i].x + items[i].width*0.5, items[i].y + items[i].height*0.5);
        cv::ellipse(frame, center, cv::Size(items[i].width*0.5, items[i].height*0.5), 0, 0, 360, cv::Scalar(255, 0, 255), 4, 8, 0);
    }

    return frame;
}

cv::Mat Detector::cannyFilter(const cv::Mat originalFrame)
{
    cv::Mat dst_frame;
    cv::Canny(originalFrame, dst_frame, 50, 200, 3);
    return dst_frame;
}

cv::Mat Detector::detectLines(const cv::Mat originalFrame)
{
    cv::Mat greyFrame, finalFrame;
    cv::Mat cannyFrame = this->cannyFilter(originalFrame);

    // Copy edges to the images that will display the results in BGR
    cv::cvtColor(cannyFrame, greyFrame, cv::COLOR_GRAY2BGR);
    finalFrame = greyFrame.clone();

    // Probabilistic Line Transform
    std::vector<cv::Vec4i> linesP; // will hold the results of the detection
    cv::HoughLinesP(cannyFrame, linesP, 1, CV_PI/180, 60, 50, 10); // runs the actual detection

    // Draw the lines
    for(size_t i = 0; i < linesP.size(); i++)
    {
        cv::Vec4i l = linesP[i];
        if(l[1]>l[3])
            cv::line(finalFrame, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 2, cv::LINE_AA);
        else
            cv::line(finalFrame, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,255,0), 2, cv::LINE_AA);
      
    }
    return finalFrame;
}