#include <opencv2/opencv.hpp>
#include <iostream>
#include "DR.h"

using namespace cv;
using namespace std;

int Lab3() noexcept {
    Mat src = imread("C:/Users/Samuel/Pictures/Cars.jpeg", 0);

    // Check if image was loaded successfully
    if (src.empty()) {
        cout << "Error: Could not load image 'image.jpg'!" << endl;
        return -1;
    }

    // Initialize min to maximum possible value for uchar (255)
    // and max to minimum possible value (0)
    int max_val = 0;
    int min_val = 255;

    // Find actual min and max values in the image
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            uchar pixel_val = src.at<uchar>(i, j);
            if (pixel_val > max_val)
                max_val = pixel_val;
            if (pixel_val < min_val)
                min_val = pixel_val;
        }
    }

    cout << "max_value:" << max_val << endl;
    cout << "min_value:" << min_val << endl;
    cout << "Dynamic Range:" << max_val - min_val << endl;

    namedWindow("img", 0);
    imshow("img", src);

    waitKey(0);
    return 0;
}