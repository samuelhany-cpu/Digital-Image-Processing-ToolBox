#ifndef IMAGEFILTERS_H
#define IMAGEFILTERS_H

#include <opencv2/opencv.hpp>

class ImageFilters {
public:
    // Lab 7: Custom Filters
    static void applyTraditionalFilter(const cv::Mat& input, cv::Mat& output, int kernelSize);
    static void applyPyramidalFilter(const cv::Mat& input, cv::Mat& output);
    static void applyCircularFilter(const cv::Mat& input, cv::Mat& output, float radius);
    static void applyConeFilter(const cv::Mat& input, cv::Mat& output);
    static void applyLaplacianFilter(const cv::Mat& input, cv::Mat& output);
    static void applySobelFilter(const cv::Mat& input, cv::Mat& output);
};

#endif // IMAGEFILTERS_H