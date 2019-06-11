#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include <opencv2/opencv.hpp>

#include "Decoder.h"
#include "Detector.h"
#include "Receiver.h"
#include "Message.h"

/**
     * @brief makeCanvas Makes composite image from the given images
     * @param vecMat Vector of Images.
     * @param windowHeight The height of the new composite image to be formed.
     * @param nRows Number of rows of images. (Number of columns will be calculated
     *              depending on the value of total number of images).
     * @return new composite image.
     */
cv::Mat makeCanvas(std::vector<cv::Mat>& vecMat, int windowHeight, int nRows)
{
    int N = vecMat.size();
    nRows  = nRows > N ? N : nRows;
    int edgeThickness = 10;
    int imagesPerRow = ceil(double(N) / nRows);
    int resizeHeight = floor(2.0 * ((floor(double(windowHeight - edgeThickness) / nRows)) / 2.0)) - edgeThickness;
    int maxRowLength = 0;

    std::vector<int> resizeWidth;
    for (int i = 0; i < N;)
    {
        int thisRowLen = 0;
        for (int k = 0; k < imagesPerRow; k++) {
            double aspectRatio = double(vecMat[i].cols) / vecMat[i].rows;
            int temp = int( ceil(resizeHeight * aspectRatio));
            resizeWidth.push_back(temp);
            thisRowLen += temp;
            if (++i == N) break;
        }
        if ((thisRowLen + edgeThickness * (imagesPerRow + 1)) > maxRowLength) {
            maxRowLength = thisRowLen + edgeThickness * (imagesPerRow + 1);
        }
    }
    int windowWidth = maxRowLength;
    cv::Mat canvasImage(windowHeight, windowWidth, CV_8UC3, cv::Scalar(0, 0, 0));

    for (int k = 0, i = 0; i < nRows; i++) {
        int y = i * resizeHeight + (i + 1) * edgeThickness;
        int x_end = edgeThickness;
        for (int j = 0; j < imagesPerRow && k < N; k++, j++) {
            int x = x_end;
            cv::Rect roi(x, y, resizeWidth[k], resizeHeight);
            cv::Size s = canvasImage(roi).size();
            // change the number of channels to three
            cv::Mat target_ROI(s, CV_8UC3);
            if (vecMat[k].channels() != canvasImage.channels()) {
                if (vecMat[k].channels() == 1) {
                    cv::cvtColor(vecMat[k], target_ROI, CV_GRAY2BGR);
                }
            } else {
                vecMat[k].copyTo(target_ROI);
            }
            cv::resize(target_ROI, target_ROI, s);
            if (target_ROI.type() != canvasImage.type()) {
                target_ROI.convertTo(target_ROI, canvasImage.type());
            }
            target_ROI.copyTo(canvasImage(roi));
            x_end += resizeWidth[k] + edgeThickness;
        }
    }
    return canvasImage;
}


int main(int argc, char* argv[])
{
    std::string broker{"localhost"};
    std::string port{"9001"};
    if (argc > 1)
    {
        broker = std::string(argv[1]);
    }
    if (argc > 2)
    {
        port = std::string(argv[2]);
    }

    Receiver receiver{broker, port, "donkeycar.training"};
    receiver.connect();

    // Create a Window
    std::string windowName = "ImageFilter";
    cv::namedWindow(windowName, cv::WINDOW_NORMAL);
    std::cout << "namedWindow" << std::endl;
    cv::moveWindow(windowName, 20, 20);
    std::cout << "moveWindow" << std::endl;

    Detector detector;
    //cv::CascadeClassifier carClassifier = detector.createClassifier("cars.xml");
    cv::Mat originalFrame;
    Decoder decoder;
    std::vector<cv::Mat> images;

    while(true)
    {
        Message message;
        std::string msg = receiver.receive();
        message = decoder.decodeMessage(msg);
        std::cout << "Decoded USER: " << message.user["angle"] << std::endl;
        std::cout << "Decoded PILOT: " << message.pilot["throttle"] << std::endl;
        std::cout << "Decoded MODE: " << message.mode << std::endl;
        std::cout << "Decoded IMG: " << message.img.substr(0, 40) << std::endl;
        auto decodedImage = decoder.decodeImage(message.img);
        originalFrame = cv::Mat(120, 160, CV_8UC3, decodedImage.data());

        if(originalFrame.empty())
        {
            std::cout << "Continue!" << std::endl;
            continue;
        }
        images.push_back(originalFrame);
        images.push_back(detector.detectLines(originalFrame));
        //images.push_back(detector.detectClassifier(carClassifier, originalFrame));

        cv::Mat canvas = makeCanvas(images, 800, 2);
        cv::imshow(windowName, canvas);
        if (cv::waitKey(500) == 27)                                                     
        {                                                                               
            std::cout << "Esc key is pressed by user. Stopping processing" << std::endl;
            break;                                                                      
        }                                                                               
        images.clear();
    }

    cv::destroyAllWindows();
    receiver.disconnect();
    return 0;
}
