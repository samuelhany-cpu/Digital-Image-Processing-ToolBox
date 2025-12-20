#ifndef IMAGEPROCESSINGLIB_H
#define IMAGEPROCESSINGLIB_H

#include <opencv2/opencv.hpp>
#include <QString>
#include <QStringList>

class ImageProcessingLib {
public:
    // Auto Enhancement
    static void applyAutoEnhance(const cv::Mat& input, cv::Mat& output, QStringList* operations = nullptr);
    
    // Basic Processing Operations
    static void convertToGrayscale(const cv::Mat& input, cv::Mat& output);
    static void applyBinaryThreshold(const cv::Mat& input, cv::Mat& output, int threshold);
    static void applyGaussianBlur(const cv::Mat& input, cv::Mat& output, int kernelSize);
    static void applyEdgeDetection(const cv::Mat& input, cv::Mat& output, double thresh1, double thresh2);
    static void invertColors(const cv::Mat& input, cv::Mat& output);
    
    // Histogram Operations
    static void applyHistogramEqualization(const cv::Mat& input, cv::Mat& output);
    static void applyOtsuThresholding(const cv::Mat& input, cv::Mat& output);
    
    // Quality Assessment
    static double calculateImageQuality(const cv::Mat& image);
    static bool isImageLowContrast(const cv::Mat& image);
    static bool isImageDark(const cv::Mat& image);
    static bool isImageBlurry(const cv::Mat& image);
};

#endif // IMAGEPROCESSINGLIB_H