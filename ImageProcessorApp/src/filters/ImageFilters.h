#ifndef IMAGEFILTERS_H
#define IMAGEFILTERS_H

#include <opencv2/opencv.hpp>

namespace ImageFilters {

/**
 * @brief Apply traditional averaging filter (mean filter)
 * @param input Input image
 * @param output Output filtered image
 * @param kernelSize Size of the filter kernel (default: 5)
 */
void applyTraditionalFilter(const cv::Mat& input, cv::Mat& output, int kernelSize = 5);

/**
 * @brief Apply pyramidal weighted filter
 * @param input Input image
 * @param output Output filtered image
 */
void applyPyramidalFilter(const cv::Mat& input, cv::Mat& output);

/**
 * @brief Apply circular averaging filter (isotropic smoothing)
 * @param input Input image
 * @param output Output filtered image
 * @param radius Filter radius (default: 2.0)
 */
void applyCircularFilter(const cv::Mat& input, cv::Mat& output, float radius = 2.0f);

/**
 * @brief Apply cone-shaped weighted filter
 * @param input Input image
 * @param output Output filtered image
 */
void applyConeFilter(const cv::Mat& input, cv::Mat& output);

/**
 * @brief Apply Laplacian edge detection filter
 * @param input Input image
 * @param output Output filtered image
 */
void applyLaplacianFilter(const cv::Mat& input, cv::Mat& output);

/**
 * @brief Apply Sobel directional edge detection filter
 * @param input Input image
 * @param output Output filtered image
 */
void applySobelFilter(const cv::Mat& input, cv::Mat& output);

} // namespace ImageFilters

#endif // IMAGEFILTERS_H
