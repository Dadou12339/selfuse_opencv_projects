#include <opencv2/opencv.hpp>
#include <iostream>

int hmin = 0, smin = 0, vmin = 0;
int hmax = 179, smax = 255, vmax = 255;
const char *tarckWindow = "颜色检测";
bool start = false;

static void onTrackbarChange(int, void *) {
    if (start){
        hmin = cv::getTrackbarPos("Hmin", tarckWindow);
        hmax = cv::getTrackbarPos("Hmax", tarckWindow);
        smin = cv::getTrackbarPos("Smin", tarckWindow);
        smax = cv::getTrackbarPos("Smax", tarckWindow);
        vmin = cv::getTrackbarPos("Vmin", tarckWindow);
        vmax = cv::getTrackbarPos("Vmax", tarckWindow);
    }
}

int main(void){
    cv::Mat img, imgHSV, mask;
    cv::VideoCapture cap(1);

    cv::namedWindow(tarckWindow);
    cv::createTrackbar("Hmin", tarckWindow, nullptr, 179, onTrackbarChange);
    cv::createTrackbar("Hmax", tarckWindow, nullptr, 179, onTrackbarChange);
    cv::createTrackbar("Smin", tarckWindow, nullptr, 255, onTrackbarChange);
    cv::createTrackbar("Smax", tarckWindow, nullptr, 255, onTrackbarChange);
    cv::createTrackbar("Vmin", tarckWindow, nullptr, 255, onTrackbarChange);
    cv::createTrackbar("Vmax", tarckWindow, nullptr, 255, onTrackbarChange);

    cv::setTrackbarPos("Hmin", tarckWindow, hmin);
    cv::setTrackbarPos("Hmax", tarckWindow, hmax);
    cv::setTrackbarPos("Smin", tarckWindow, smin);
    cv::setTrackbarPos("Smax", tarckWindow, smax);
    cv::setTrackbarPos("Vmin", tarckWindow, vmin);
    cv::setTrackbarPos("Vmax", tarckWindow, vmax);

    start = true;
    while (true){
        cap.read(img);
        cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);

        cv::Scalar lower(hmin, smin, vmin);
        cv::Scalar upper(hmax, smax, vmax);
        cv::inRange(imgHSV, lower, upper, mask);

        imshow("Image", img);
        imshow("Mask", mask);

        if (cv::waitKey(1) == 'q'){
            break;
        }
    }

    return 0;
}