#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "Decoder.h"
#include "Detector.h"
#include "Receiver.h"


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
    std::string broker{"192.168.1.41"};
    std::string port{"5559"};
    if (argc > 1)
    {
        broker = std::string(argv[1]);
    }
    if (argc > 2)
    {
        port = std::string(argv[2]);
    }

    Receiver receiver{broker, port, "donkeycar.camera"};
    receiver.connect();

    // Create a Window
    std::string windowName = "ImageFilter";
    cv::namedWindow(windowName, cv::WINDOW_NORMAL);
    cv::moveWindow(windowName, 20, 20);

    Detector detector;
    //cv::CascadeClassifier carClassifier = detector.createClassifier("../res/cars.xml");
    cv::Mat originalFrame;
    std::string decodedMsg;
    Decoder decoder;
    std::vector<cv::Mat> images;

    while(true)
    {
        std::string msg = receiver.receive();
        decodedMsg = decoder.decodeBase64(msg);
        originalFrame = imdecode(std::vector<uchar>(decodedMsg.begin(), decodedMsg.end()), cv::IMREAD_UNCHANGED);

        if(originalFrame.empty()){
            continue;
        }
        images.push_back(originalFrame);
        //images.push_back(detector.detectClassifier(carClassifier, originalFrame));
        images.push_back(detector.cannyFilter(originalFrame));
        images.push_back(detector.detectLines(originalFrame));

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
