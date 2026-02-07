#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using std::vector;

// Hmin, Hmax, Smin, Smax, Vmin, Vmax
const vector<vector<int>> COLORS = {
    {0, 179, 91, 255, 45, 140}, //红色
    {56, 90, 120, 255, 0, 94}, //绿色
};

const vector<cv::Scalar> myColor = {
    {0, 0, 255},
    {0, 255, 0}
};

cv::VideoCapture cap(1);
cv::Mat frame;
vector<vector<int>> points;

namespace Func{
    static cv::Point findPoint(cv::Mat &img){
        vector<vector<cv::Point>> contours;
        vector<cv::Vec4i> hierarchy;
        cv::findContours(img, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        int sz = contours.size();
        vector<vector<cv::Point>> conPoly(sz);
        vector<cv::Rect> boundRect(sz);

        cv::Point res;
        for (int i=0; i<sz; i++){
            float area = cv::contourArea(contours[i]);
            if (area > 1000){
                float arc = cv::arcLength(contours[i], true);
                cv::approxPolyDP(contours[i], conPoly[i], 0.02*arc, true);
                boundRect[i] = cv::boundingRect(conPoly[i]);

                res.x = boundRect[i].x + boundRect[i].width / 2;
                res.y = boundRect[i].y;

                drawContours(frame, conPoly, i, cv::Scalar(255, 0, 255), 2);
			    rectangle(frame, boundRect[i].tl(), boundRect[i].br(), cv::Scalar(0, 255, 0), 5);
            }
        }

        return res;
    }

    static void findColor(const cv::Mat &img){
        cv::Mat imgHSV;
        cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);
        for (int i=0, sz=COLORS.size(); i<sz; i++){
            cv::Scalar lower(COLORS[i][0], COLORS[i][2], COLORS[i][4]);
            cv::Scalar upper(COLORS[i][1], COLORS[i][3], COLORS[i][5]);
            cv::Mat mask;
            cv::inRange(imgHSV, lower, upper, mask);

            cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
            cv::erode(mask, mask, kernel);
            cv::dilate(mask, mask, kernel);

            cv::Point myPoint = findPoint(mask);
            if (myPoint.x != 0){
                points.push_back(vector<int>{myPoint.x, myPoint.y, i});
            }
        }
    }

    static void drawOnCanvas(const vector<vector<int>> &points){
        for (int i=0, sz = points.size(); i<sz; i++)
        {
            circle(frame, cv::Point(points[i][0],points[i][1]), 10, myColor[points[i][2]], cv::FILLED);
        }
    }
}

int main(void){
    while (true){
        cap.read(frame);
        Func::findColor(frame);
        Func::drawOnCanvas(points);

        cv::imshow("虚拟画布", frame);

        if (cv::waitKey(1) == 'q'){
            break;
        }
    }

    return 0;
}