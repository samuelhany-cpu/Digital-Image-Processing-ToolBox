#include "ImageFilters.h"
#include <opencv2/photo.hpp>
#include <cmath>
#include <algorithm>
#include <random>

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

// =============================================================================
// PHASE 1: NOISE ADDITION
// =============================================================================

void addGaussianNoise(const cv::Mat& input, cv::Mat& output, double mean, double stddev) {
    if (!isValidImage(input)) return;
    
    cv::Mat noise = cv::Mat(input.size(), input.type());
    cv::randn(noise, mean, stddev);
    
    cv::Mat temp;
    input.convertTo(temp, CV_16SC3);
    noise.convertTo(noise, CV_16SC3);
    
    cv::add(temp, noise, temp);
    temp.convertTo(output, input.type());
}

void addSaltPepperNoise(const cv::Mat& input, cv::Mat& output, double density) {
    if (!isValidImage(input)) return;
    
    output = input.clone();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    int numPixels = input.rows * input.cols;
    int numNoisePixels = static_cast<int>(numPixels * density);
    
    for (int i = 0; i < numNoisePixels; ++i) {
        int row = static_cast<int>(dis(gen) * input.rows);
        int col = static_cast<int>(dis(gen) * input.cols);
        
        if (dis(gen) < 0.5) {
            // Salt (white)
            if (output.channels() == 1) {
                output.at<uchar>(row, col) = 255;
            } else {
                output.at<cv::Vec3b>(row, col) = cv::Vec3b(255, 255, 255);
            }
        } else {
            // Pepper (black)
            if (output.channels() == 1) {
                output.at<uchar>(row, col) = 0;
            } else {
                output.at<cv::Vec3b>(row, col) = cv::Vec3b(0, 0, 0);
            }
        }
    }
}

void addPoissonNoise(const cv::Mat& input, cv::Mat& output) {
    if (!isValidImage(input)) return;
    
    cv::Mat temp;
    input.convertTo(temp, CV_32F);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    
    for (int i = 0; i < temp.rows; ++i) {
        for (int j = 0; j < temp.cols; ++j) {
            if (temp.channels() == 1) {
                float val = temp.at<float>(i, j);
                if (val > 0) {
                    std::poisson_distribution<int> poisson(val);
                    temp.at<float>(i, j) = static_cast<float>(poisson(gen));
                }
            } else {
                cv::Vec3f& pixel = temp.at<cv::Vec3f>(i, j);
                for (int c = 0; c < 3; ++c) {
                    if (pixel[c] > 0) {
                        std::poisson_distribution<int> poisson(static_cast<int>(pixel[c]));
                        pixel[c] = static_cast<float>(poisson(gen));
                    }
                }
            }
        }
    }
    
    temp.convertTo(output, input.type());
}

void addSpeckleNoise(const cv::Mat& input, cv::Mat& output, double variance) {
    if (!isValidImage(input)) return;
    
    cv::Mat noise = cv::Mat(input.size(), CV_32F);
    cv::randn(noise, 0.0, std::sqrt(variance));
    
    cv::Mat temp;
    input.convertTo(temp, CV_32F);
    
    // Speckle: I + I * noise
    for (int i = 0; i < temp.rows; ++i) {
        for (int j = 0; j < temp.cols; ++j) {
            if (temp.channels() == 1) {
                temp.at<float>(i, j) += temp.at<float>(i, j) * noise.at<float>(i, j);
            } else {
                cv::Vec3f noiseVec(noise.at<float>(i, j), noise.at<float>(i, j), noise.at<float>(i, j));
                cv::Vec3f& pixel = temp.at<cv::Vec3f>(i, j);
                pixel += pixel.mul(noiseVec);
            }
        }
    }
    
    temp.convertTo(output, input.type());
}

// =============================================================================
// PHASE 1: ADVANCED DENOISING FILTERS
// =============================================================================

void applyMedianFilter(const cv::Mat& input, cv::Mat& output, int kernelSize) {
    if (!isValidImage(input)) return;
    
    // Ensure kernel size is odd
    if (kernelSize % 2 == 0) kernelSize++;
    
    // Clamp to valid range
    kernelSize = std::max(3, std::min(9, kernelSize));
    
    cv::medianBlur(input, output, kernelSize);
}

void applyBilateralFilter(const cv::Mat& input, cv::Mat& output, 
                          int d, double sigmaColor, double sigmaSpace) {
    if (!isValidImage(input)) return;
    
    cv::bilateralFilter(input, output, d, sigmaColor, sigmaSpace);
}

void applyNonLocalMeansDenoising(const cv::Mat& input, cv::Mat& output,
                                 float h, int templateWindowSize, 
                                 int searchWindowSize) {
    if (!isValidImage(input)) return;
    
    if (input.channels() == 1) {
        cv::fastNlMeansDenoising(input, output, h, templateWindowSize, searchWindowSize);
    } else {
        cv::fastNlMeansDenoisingColored(input, output, h, h, templateWindowSize, searchWindowSize);
    }
}

void applyMorphologicalOpening(const cv::Mat& input, cv::Mat& output, 
                               int kernelSize, int kernelShape) {
    if (!isValidImage(input)) return;
    
    cv::Mat kernel = createStructuringElement(kernelShape, kernelSize);
    cv::morphologyEx(input, output, cv::MORPH_OPEN, kernel);
}

void applyMorphologicalClosing(const cv::Mat& input, cv::Mat& output, 
                               int kernelSize, int kernelShape) {
    if (!isValidImage(input)) return;
    
    cv::Mat kernel = createStructuringElement(kernelShape, kernelSize);
    cv::morphologyEx(input, output, cv::MORPH_CLOSE, kernel);
}

void applyMorphologicalGradient(const cv::Mat& input, cv::Mat& output, int kernelSize) {
    if (!isValidImage(input)) return;
    
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, 
                                               cv::Size(kernelSize, kernelSize));
    cv::morphologyEx(input, output, cv::MORPH_GRADIENT, kernel);
}

void applyTopHat(const cv::Mat& input, cv::Mat& output, int kernelSize) {
    if (!isValidImage(input)) return;
    
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, 
                                               cv::Size(kernelSize, kernelSize));
    cv::morphologyEx(input, output, cv::MORPH_TOPHAT, kernel);
}

void applyBlackHat(const cv::Mat& input, cv::Mat& output, int kernelSize) {
    if (!isValidImage(input)) return;
    
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, 
                                               cv::Size(kernelSize, kernelSize));
    cv::morphologyEx(input, output, cv::MORPH_BLACKHAT, kernel);
}

// =============================================================================
// PHASE 1: SHARPENING FILTERS
// =============================================================================

void applyUnsharpMask(const cv::Mat& input, cv::Mat& output, 
                      double sigma, double amount, int threshold) {
    if (!isValidImage(input)) return;
    
    // Create blurred version
    cv::Mat blurred;
    cv::GaussianBlur(input, blurred, cv::Size(0, 0), sigma);
    
    // Calculate the sharp mask
    cv::Mat sharpMask;
    cv::subtract(input, blurred, sharpMask);
    
    // Apply threshold if specified
    if (threshold > 0) {
        cv::Mat mask = cv::abs(sharpMask) >= threshold;
        sharpMask.setTo(0, ~mask);
    }
    
    // Add the weighted mask to the original
    cv::Mat temp;
    input.convertTo(temp, CV_32F);
    sharpMask.convertTo(sharpMask, CV_32F);
    
    temp = temp + amount * sharpMask;
    temp.convertTo(output, input.type());
}

void applyHighPassFilter(const cv::Mat& input, cv::Mat& output, int kernelSize) {
    if (!isValidImage(input)) return;
    
    // Ensure kernel size is odd
    if (kernelSize % 2 == 0) kernelSize++;
    
    // Create blurred version (low-pass)
    cv::Mat blurred;
    cv::GaussianBlur(input, blurred, cv::Size(kernelSize, kernelSize), 0);
    
    // High-pass = Original - Low-pass
    cv::Mat temp1, temp2;
    input.convertTo(temp1, CV_16S);
    blurred.convertTo(temp2, CV_16S);
    
    cv::Mat highPass;
    cv::subtract(temp1, temp2, highPass);
    
    // Normalize to 0-255 range
    cv::normalize(highPass, output, 0, 255, cv::NORM_MINMAX, CV_8U);
}

void applyCustomSharpen(const cv::Mat& input, cv::Mat& output, int strength) {
    if (!isValidImage(input)) return;
    
    // Clamp strength to 0-200
    strength = std::max(0, std::min(200, strength));
    
    // Calculate sharpening amount (1.0 = 100% = normal)
    double amount = strength / 100.0;
    
    // Sharpening kernel (standard 3x3)
    cv::Mat kernel = (cv::Mat_<float>(3, 3) << 
        0, -1, 0,
        -1, 5, -1,
        0, -1, 0);
    
    // Scale kernel based on strength
    kernel = (kernel - 1) * amount + 1;
    
    cv::filter2D(input, output, -1, kernel);
}

// =============================================================================
// UTILITY FUNCTIONS
// =============================================================================

bool isValidImage(const cv::Mat& image) {
    return !image.empty() && (image.channels() == 1 || image.channels() == 3);
}

cv::Mat createStructuringElement(int shape, int size) {
    cv::MorphShapes morphShape;
    
    switch (shape) {
        case 0:
            morphShape = cv::MORPH_RECT;
            break;
        case 1:
            morphShape = cv::MORPH_ELLIPSE;
            break;
        case 2:
            morphShape = cv::MORPH_CROSS;
            break;
        default:
            morphShape = cv::MORPH_ELLIPSE;
    }
    
    return cv::getStructuringElement(morphShape, cv::Size(size, size));
}

} // namespace ImageFilters
