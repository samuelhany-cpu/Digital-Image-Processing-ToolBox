#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int Lab4()
{
    ///////////////////Image Translation///////////////////
    Mat src_t, dst_t;
    src_t = imread("C:/Users/Samuel/Pictures/Cars.jpeg", 0);
    namedWindow("original_t", 0);
    imshow("original_t", src_t);
    waitKey(0);
    int tx = 50, ty = 50;
    Mat TM = (Mat_<float>(2, 3) << 1, 0, tx, 0, 1, ty);
    warpAffine(src_t, dst_t, TM, src_t.size());
    namedWindow("translation", 0);
    imshow("translation", dst_t);
    imwrite("translation.jpg", dst_t);
    waitKey(0);

    ///////////////////Image Rotation///////////////////
    Mat src_r, dst_r;
    src_r = imread("C:/Users/Samuel/Pictures/Cars.jpeg", 0);
    namedWindow("original_r", 0);
    imshow("original_r", src_r);
    waitKey(0);

    // resize(src_r, src_r, Size(src_r.cols, src_r.cols));
    Mat R = getRotationMatrix2D(Point2f(src_r.cols / 2, src_r.rows / 2), 90, 1);
    warpAffine(src_r, dst_r, R, src_r.size());
    namedWindow("rotation", 0);
    imshow("rotation", dst_r);
    waitKey(0);
    imwrite("rotation.jpg", dst_r);

    ///////////////////Image Skewing///////////////////
    Mat src_s, dst_s;
    src_s = imread("C:/Users/Samuel/Pictures/Cars.jpeg", 0);
    namedWindow("original_s", 0);
    imshow("original_s", src_s);
    waitKey(0);
    Point2f src_p[3];
    src_p[0] = Point2f(0, 0);
    src_p[1] = Point2f(src_s.cols - 1, 0);
    src_p[2] = Point2f(100, src_s.rows - 1);
    Point2f dst_p[3];
    dst_p[0] = Point2f(0, 0);
    dst_p[1] = Point2f(src_s.cols - 1, 0);
    dst_p[2] = Point2f(0, src_s.rows - 1);
    Mat SM = getAffineTransform(src_p, dst_p);
    warpAffine(src_s, dst_s, SM, src_s.size());
    namedWindow("skewing", 0);
    imshow("skewing", dst_s);
    imwrite("skewing.jpg", dst_s);
    waitKey(0);

    ///////////////////Image Zoom///////////////////
    Mat src_z, dst_z;
    src_z = imread("C:/Users/Samuel/Pictures/Cars.jpeg", 0);
    namedWindow("original_z", 0);
    imshow("original_z", src_z);
    waitKey(0);
    Mat src_zoom = src_z(Rect(0, 0, src_z.cols / 2, src_z.rows / 2));
    resize(src_zoom, dst_z, Size(), 2, 2, 0);
    namedWindow("image zoom", 0);
    imshow("image zoom", dst_z);
    waitKey(0);
    imwrite("zoom.jpg", dst_z);

    ///////////////////Image Flip///////////////////
    Mat src_f, dst_f;
    src_f = imread("C:/Users/Samuel/Pictures/Cars.jpeg", 0);
    namedWindow("original_f", 0);
    imshow("original_f", src_f);
    waitKey(0);
    flip(src_f, dst_f, 0);
    namedWindow("flip around x", 0);
    imshow("flip around x", dst_f);
    imwrite("flip around x.jpg", dst_f);
    waitKey(0);
    flip(src_f, dst_f, 1);
    namedWindow("flip around y", 0);
    imshow("flip around y", dst_f);
    imwrite("flip around y.jpg", dst_f);
    waitKey(0);
    flip(src_f, dst_f, -1);
    namedWindow("flip around xy", 0);
    imshow("flip around xy", dst_f);
    imwrite("flip around xy.jpg", dst_f);
    waitKey(0);

    return 0;
}