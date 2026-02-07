#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using std::vector;

int main(void) {
	cv::Mat img;
	cv::VideoCapture cap(1);

	cv::CascadeClassifier plateCascade;
	plateCascade.load("../xml/haarcascade_russian_plate_number.xml");

	if (plateCascade.empty()) { 
        std::cout << "xml文件加载失败" << std::endl; 
    }

	vector<cv::Rect> plates;

	while (true) {
		cap.read(img);
		plateCascade.detectMultiScale(img, plates, 1.1, 10);

		for (int i = 0; i < plates.size(); i++)
		{
			rectangle(img, plates[i].tl(), plates[i].br(), cv::Scalar(255, 0, 255), 3);
		}

		imshow("Image", img);
		if (cv::waitKey(1) == 'q'){
            break;
        }
	}

    return 0;
}