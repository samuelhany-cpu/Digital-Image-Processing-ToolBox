#ifndef IMAGEPROCESSINGLIB_H
#define IMAGEPROCESSINGLIB_H

#include <opencv2/opencv.hpp>

namespace ImageProcessingLib {

/**
 * @brief Convert image to grayscale
 * @param input Input color image
 * @param output Output grayscale image
 */
void convertToGrayscale(const cv::Mat& input, cv::Mat& output);

/**
 * @brief Apply binary threshold to image
 * @param input Input image
 * @param output Output thresholded image
 * @param threshold Threshold value (default: 128)
 */
void applyBinaryThreshold(const cv::Mat& input, cv::Mat& output, int threshold = 128);

/**
 * @brief Apply Gaussian blur filter
 * @param input Input image
 * @param output Output blurred image
 * @param kernelSize Kernel size (default: 5)
 */
void applyGaussianBlur(const cv::Mat& input, cv::Mat& output, int kernelSize = 5);

/**
 * @brief Apply Canny edge detection
 * @param input Input image
 * @param output Output edge map
 * @param lowThreshold Low threshold for edge detection (default: 100)
 * @param highThreshold High threshold for edge detection (default: 200)
 */
void applyEdgeDetection(const cv::Mat& input, cv::Mat& output, 
                       int lowThreshold = 100, int highThreshold = 200);

/**
 * @brief Invert image colors
 * @param input Input image
 * @param output Output inverted image
 */
void invertColors(const cv::Mat& input, cv::Mat& output);

/**
 * @brief Apply histogram equalization
 * @param input Input image
 * @param output Output equalized image
 */
void applyHistogramEqualization(const cv::Mat& input, cv::Mat& output);

/**
 * @brief Apply Otsu's automatic thresholding
 * @param input Input image
 * @param output Output thresholded image
 */
void applyOtsuThresholding(const cv::Mat& input, cv::Mat& output);

} // namespace ImageProcessingLib

#endif // IMAGEPROCESSINGLIB_H
