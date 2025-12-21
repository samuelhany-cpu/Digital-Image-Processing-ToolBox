#ifndef IMAGEFILTERS_H
#define IMAGEFILTERS_H

#include <opencv2/opencv.hpp>

namespace ImageFilters {

// =============================================================================
// BASIC FILTERS (Already Implemented)
// =============================================================================

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

// =============================================================================
// PHASE 1: NOISE ADDITION (for testing/simulation)
// =============================================================================

/**
 * @brief Add Gaussian noise to image
 * @param input Input image
 * @param output Output noisy image
 * @param mean Mean of the Gaussian distribution (default: 0)
 * @param stddev Standard deviation of the Gaussian distribution (default: 25)
 */
void addGaussianNoise(const cv::Mat& input, cv::Mat& output, double mean = 0.0, double stddev = 25.0);

/**
 * @brief Add Salt and Pepper noise to image
 * @param input Input image
 * @param output Output noisy image
 * @param density Noise density (0.0 to 1.0, default: 0.05 = 5%)
 */
void addSaltPepperNoise(const cv::Mat& input, cv::Mat& output, double density = 0.05);

/**
 * @brief Add Poisson noise to image (photon counting noise)
 * @param input Input image
 * @param output Output noisy image
 */
void addPoissonNoise(const cv::Mat& input, cv::Mat& output);

/**
 * @brief Add Speckle noise to image (multiplicative noise)
 * @param input Input image
 * @param output Output noisy image
 * @param variance Variance of the speckle noise (default: 0.1)
 */
void addSpeckleNoise(const cv::Mat& input, cv::Mat& output, double variance = 0.1);

// =============================================================================
// PHASE 1: ADVANCED DENOISING FILTERS
// =============================================================================

/**
 * @brief Apply median filter (excellent for salt-and-pepper noise)
 * @param input Input image
 * @param output Output filtered image
 * @param kernelSize Kernel size (3, 5, 7, or 9, default: 5)
 */
void applyMedianFilter(const cv::Mat& input, cv::Mat& output, int kernelSize = 5);

/**
 * @brief Apply bilateral filter (edge-preserving smoothing)
 * @param input Input image
 * @param output Output filtered image
 * @param d Diameter of pixel neighborhood (default: 9)
 * @param sigmaColor Filter sigma in color space (default: 75)
 * @param sigmaSpace Filter sigma in coordinate space (default: 75)
 */
void applyBilateralFilter(const cv::Mat& input, cv::Mat& output, 
                          int d = 9, double sigmaColor = 75.0, double sigmaSpace = 75.0);

/**
 * @brief Apply Non-Local Means denoising
 * @param input Input image
 * @param output Output filtered image
 * @param h Filter strength (default: 10)
 * @param templateWindowSize Template patch size (default: 7)
 * @param searchWindowSize Search area size (default: 21)
 */
void applyNonLocalMeansDenoising(const cv::Mat& input, cv::Mat& output,
                                 float h = 10.0f, int templateWindowSize = 7, 
                                 int searchWindowSize = 21);

/**
 * @brief Apply morphological opening (erosion followed by dilation)
 * @param input Input image
 * @param output Output filtered image
 * @param kernelSize Kernel size (default: 5)
 * @param kernelShape 0=Rectangle, 1=Ellipse, 2=Cross (default: 1)
 */
void applyMorphologicalOpening(const cv::Mat& input, cv::Mat& output, 
                               int kernelSize = 5, int kernelShape = 1);

/**
 * @brief Apply morphological closing (dilation followed by erosion)
 * @param input Input image
 * @param output Output filtered image
 * @param kernelSize Kernel size (default: 5)
 * @param kernelShape 0=Rectangle, 1=Ellipse, 2=Cross (default: 1)
 */
void applyMorphologicalClosing(const cv::Mat& input, cv::Mat& output, 
                               int kernelSize = 5, int kernelShape = 1);

/**
 * @brief Apply morphological gradient (dilation - erosion)
 * @param input Input image
 * @param output Output filtered image
 * @param kernelSize Kernel size (default: 5)
 */
void applyMorphologicalGradient(const cv::Mat& input, cv::Mat& output, int kernelSize = 5);

/**
 * @brief Apply top-hat transform (original - opening)
 * @param input Input image
 * @param output Output filtered image
 * @param kernelSize Kernel size (default: 9)
 */
void applyTopHat(const cv::Mat& input, cv::Mat& output, int kernelSize = 9);

/**
 * @brief Apply black-hat transform (closing - original)
 * @param input Input image
 * @param output Output filtered image
 * @param kernelSize Kernel size (default: 9)
 */
void applyBlackHat(const cv::Mat& input, cv::Mat& output, int kernelSize = 9);

// =============================================================================
// PHASE 1: SHARPENING FILTERS
// =============================================================================

/**
 * @brief Apply unsharp masking for image sharpening
 * @param input Input image
 * @param output Output sharpened image
 * @param sigma Gaussian blur sigma (default: 1.0)
 * @param amount Sharpening amount (default: 1.5)
 * @param threshold Threshold for sharpening (default: 0)
 */
void applyUnsharpMask(const cv::Mat& input, cv::Mat& output, 
                      double sigma = 1.0, double amount = 1.5, int threshold = 0);

/**
 * @brief Apply high-pass filter for edge enhancement
 * @param input Input image
 * @param output Output filtered image
 * @param kernelSize Gaussian kernel size for blur (default: 21)
 */
void applyHighPassFilter(const cv::Mat& input, cv::Mat& output, int kernelSize = 21);

/**
 * @brief Apply custom sharpening kernel
 * @param input Input image
 * @param output Output sharpened image
 * @param strength Sharpening strength (0-200, default: 100)
 */
void applyCustomSharpen(const cv::Mat& input, cv::Mat& output, int strength = 100);

// =============================================================================
// UTILITY FUNCTIONS
// =============================================================================

/**
 * @brief Check if image is valid for filtering
 * @param image Input image
 * @return true if valid, false otherwise
 */
bool isValidImage(const cv::Mat& image);

/**
 * @brief Create morphological structuring element
 * @param shape 0=Rectangle, 1=Ellipse, 2=Cross
 * @param size Kernel size
 * @return Structuring element
 */
cv::Mat createStructuringElement(int shape, int size);

} // namespace ImageFilters

#endif // IMAGEFILTERS_H
