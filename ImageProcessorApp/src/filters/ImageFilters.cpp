#include "ImageFilters.h"
#include <cmath>
#include <algorithm>

namespace ImageFilters {

void applyTraditionalFilter(const cv::Mat& input, cv::Mat& output, int kernelSize) {
    // Traditional averaging filter with equal weights
    cv::Mat kernel = cv::Mat::ones(kernelSize, kernelSize, CV_32F) / float(kernelSize * kernelSize);
    cv::filter2D(input, output, -1, kernel);
}

void applyPyramidalFilter(const cv::Mat& input, cv::Mat& output) {
    // Pyramidal filter with weights increasing toward center
    cv::Mat kernel = (cv::Mat_<float>(5, 5) << 
        1, 2, 3, 2, 1,
        2, 3, 4, 3, 2,
        3, 4, 6, 4, 3,
        2, 3, 4, 3, 2,
        1, 2, 3, 2, 1);
    
    // Normalize kernel
    kernel = kernel / cv::sum(kernel)[0];
    cv::filter2D(input, output, -1, kernel);
}

void applyCircularFilter(const cv::Mat& input, cv::Mat& output, float radius) {
    // Circular filter - only pixels within radius get weighted
    int kernelSize = static_cast<int>(radius * 2) + 1;
    cv::Mat kernel = cv::Mat::zeros(kernelSize, kernelSize, CV_32F);
    cv::Point center(kernelSize / 2, kernelSize / 2);
    
    // Calculate circular mask
    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            float dist = std::sqrt(std::pow(i - center.y, 2) + std::pow(j - center.x, 2));
            if (dist <= radius) {
                kernel.at<float>(i, j) = 1.0f;
            }
        }
    }
    
    // Normalize kernel
    kernel = kernel / cv::sum(kernel)[0];
    cv::filter2D(input, output, -1, kernel);
}

void applyConeFilter(const cv::Mat& input, cv::Mat& output) {
    // Cone filter - weights decrease linearly from center
    int kernelSize = 5;
    cv::Mat kernel = cv::Mat::zeros(kernelSize, kernelSize, CV_32F);
    cv::Point center(kernelSize / 2, kernelSize / 2);
    float maxDist = std::sqrt(2 * center.x * center.x);
    
    // Calculate cone-shaped weights
    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            float dist = std::sqrt(std::pow(i - center.y, 2) + std::pow(j - center.x, 2));
            kernel.at<float>(i, j) = std::max(0.0f, maxDist - dist);
        }
    }
    
    // Normalize kernel
    kernel = kernel / cv::sum(kernel)[0];
    cv::filter2D(input, output, -1, kernel);
}

void applyLaplacianFilter(const cv::Mat& input, cv::Mat& output) {
    // Laplacian filter kernel (3x3)
    cv::Mat kernel_L = (cv::Mat_<float>(3, 3) << 
        1, 1, 1,
        1, -8, 1,
        1, 1, 1);
    
    cv::Mat dst_Lap;
    cv::filter2D(input, dst_Lap, CV_8UC1, kernel_L);
    
    // Normalize for better visualization
    cv::normalize(dst_Lap, output, 0, 255, cv::NORM_MINMAX, CV_8U);
}

void applySobelFilter(const cv::Mat& input, cv::Mat& output) {
    // Sobel filter kernels
    cv::Mat kernel_TH = (cv::Mat_<int>(3, 3) << 
        -1, -2, -1,
        0, 0, 0,
        1, 2, 1);
    
    cv::Mat kernel_TV = (cv::Mat_<int>(3, 3) << 
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1);
    
    cv::Mat kernel_Td = (cv::Mat_<int>(3, 3) << 
        2, 1, 0,
        1, 0, -1,
        0, -1, -2);
    
    cv::Mat dstS_H, dstS_V, dstS_D, dstS_HV, dstS_S;
    
    // Apply filters
    cv::filter2D(input, dstS_H, CV_8UC1, kernel_TH);
    cv::filter2D(input, dstS_V, CV_8UC1, kernel_TV);
    cv::filter2D(input, dstS_D, CV_8UC1, kernel_Td);
    
    // Combine horizontal and vertical
    cv::addWeighted(dstS_H, 1, dstS_V, 1, 0, dstS_HV);
    cv::addWeighted(dstS_HV, 1, dstS_D, 1, 0, dstS_S);
    
    // Normalize for better visualization
    cv::normalize(dstS_S, output, 0, 255, cv::NORM_MINMAX, CV_8U);
}

} // namespace ImageFilters
