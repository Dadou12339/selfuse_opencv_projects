#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>

using std::vector;

const int W = 500, H = 600;
cv::Mat imgOriginal, imgGray, imgBlur, imgCanny, imgDil, imgWarp, imgCrop;

namespace Func{
    static void preProcessing (const cv::Mat &img){
        cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(imgGray, imgBlur, cv::Size(3, 3), 3);
        cv::Canny(imgBlur, imgCanny, 158, 255);

        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
        cv::dilate(imgCanny, imgDil, kernel);
    }

    static vector<cv::Point> getContours(cv::Mat &img){
        vector<vector<cv::Point>> contours;
        vector<cv::Vec4i> hierarchy;

        cv::findContours(img, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        vector<vector<cv::Point>> conPoly(contours.size());
        vector<cv::Point> biggest;

        int maxArea = 0;
        for (int i=0, sz=contours.size(); i<sz; i++){
            int area = cv::contourArea(contours[i]);

            if (area > 1000){
                float peri = cv::arcLength(contours[i], true);
                cv::approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

                if (area > maxArea && conPoly[i].size()==4 ) {
                    biggest = { conPoly[i][0],conPoly[i][1] ,conPoly[i][2] ,conPoly[i][3] };
                    maxArea = area;
                }

            }
        }

        return biggest;
    }

    static void drawPoints(cv::Mat &img, vector<cv::Point> points, cv::Scalar color){
        for (int i=0, sz=points.size(); i<sz; i++){
            cv::circle(img, points[i], 10, color, cv::FILLED);
            cv::putText(img, std::to_string(i), points[i], cv::FONT_HERSHEY_PLAIN, 4, color, 4);
        }
    }

    static vector<cv::Point> reorder(const vector<cv::Point> &points){
        vector<cv::Point> res;
        vector<int> sub, del;
        for (auto &point : points){
            sub.push_back(point.x + point.y);
            del.push_back(point.x - point.y);
        }

        res.push_back(points[std::min_element(sub.begin(), sub.end()) - sub.begin()]);
        res.push_back(points[std::max_element(del.begin(), del.end()) - del.begin()]);
        res.push_back(points[std::min_element(del.begin(), del.end()) - del.begin()]);
        res.push_back(points[std::max_element(sub.begin(), sub.end()) - sub.begin()]);

        return res;
    }

    static cv::Mat warpImg(const cv::Mat &img, vector<cv::Point> points, float w, float h){
        cv::Point2f src[] = {points[0], points[1], points[2], points[3]};
        cv::Point2f dst[] = {{0, 0}, {w, 0}, {0, h}, {w, h}};

        cv::Mat warp = cv::getPerspectiveTransform(src, dst);

        cv::Mat res;
        cv::warpPerspective(img, res, warp, cv::Size(w, h));

        return res;
    }
}

int main(void){
    const char *path = "../img/paper.jpg";
    imgOriginal = cv::imread(path);
    cv::resize(imgOriginal, imgOriginal, cv::Size(), 0.5, 0.5);

    Func::preProcessing(imgOriginal);

    vector<cv::Point> points = Func::getContours(imgDil);
    points = Func::reorder(points);
    // Func::drawPoints(imgOriginal, points, cv::Scalar(0, 0, 255));

    imgWarp = Func::warpImg(imgOriginal, points, W, H);

    cv::imshow("Image", imgWarp);

    cv::waitKey(0);

    return 0;
}