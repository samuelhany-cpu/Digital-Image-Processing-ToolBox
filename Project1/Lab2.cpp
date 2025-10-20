#include <opencv2/opencv.hpp>
#include <iostream>
#include "BW.h"
using namespace std;
using namespace cv;

int Lab2() noexcept {
	Mat img = imread("C:/Users/Samuel/Pictures/Cars.jpeg", 0);
	cout << "Pixel Intensities = \n" << img << endl;
	cout << "img rows = " << img.rows << endl;
	cout << "img cols = " << img.cols << endl;
	cout << "Total no. of pixels = " << img.total() << endl;
	cout << "Image Channels = " << img.channels() << endl;
	cout << "Image Pixel depth = " << img.depth() << endl;
	int pixel_val = img.at<uchar>(0, 0);
	cout << "Pixel Value (0,0) = " << pixel_val << endl;
	namedWindow("Image Info", 0);
	imshow("Image Info", img);
	imwrite("Image Info.jpg", img); //save
	waitKey(0);
	return 0;
}