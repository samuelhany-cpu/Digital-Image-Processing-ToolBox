#ifndef MORPHOLOGYLIB_H
#define MORPHOLOGYLIB_H

#include <opencv2/opencv.hpp>
#include <vector>

/**
 * @brief Library for morphological operations and advanced edge detection
 * 
 * Provides comprehensive morphological image processing operations including
 * basic operations (erosion, dilation), compound operations (opening, closing),
 * advanced morphology (gradient, top-hat, black-hat), custom structuring elements,
 * and specialized edge detection operators.
 */
class MorphologyLib {
public:
    /**
     * @brief Structuring element shapes
     */
    enum StructuringElementShape {
        RECT = cv::MORPH_RECT,      ///< Rectangular structuring element
        ELLIPSE = cv::MORPH_ELLIPSE,///< Elliptical structuring element
        CROSS = cv::MORPH_CROSS     ///< Cross-shaped structuring element
    };

    // ==========================================================================
    // BASIC MORPHOLOGICAL OPERATIONS
    // ==========================================================================

    /**
     * @brief Erode image - shrink bright regions
     * @param input Source image
     * @param output Eroded image
     * @param kernelSize Size of structuring element (must be odd)
     * @param shape Shape of structuring element
     * @param iterations Number of times erosion is applied
     */
    static void applyErosion(const cv::Mat& input, cv::Mat& output, 
                            int kernelSize = 5, 
                            StructuringElementShape shape = ELLIPSE,
                            int iterations = 1);

    /**
     * @brief Dilate image - expand bright regions
     * @param input Source image
     * @param output Dilated image
     * @param kernelSize Size of structuring element (must be odd)
     * @param shape Shape of structuring element
     * @param iterations Number of times dilation is applied
     */
    static void applyDilation(const cv::Mat& input, cv::Mat& output, 
                             int kernelSize = 5,
                             StructuringElementShape shape = ELLIPSE,
                             int iterations = 1);

    /**
     * @brief Opening operation (erosion followed by dilation) - remove small bright spots
     * @param input Source image
     * @param output Result image
     * @param kernelSize Size of structuring element
     * @param shape Shape of structuring element
     */
    static void applyOpening(const cv::Mat& input, cv::Mat& output, 
                            int kernelSize = 5,
                            StructuringElementShape shape = ELLIPSE);

    /**
     * @brief Closing operation (dilation followed by erosion) - fill small dark holes
     * @param input Source image
     * @param output Result image
     * @param kernelSize Size of structuring element
     * @param shape Shape of structuring element
     */
    static void applyClosing(const cv::Mat& input, cv::Mat& output, 
                            int kernelSize = 5,
                            StructuringElementShape shape = ELLIPSE);

    /**
     * @brief Morphological gradient (dilation - erosion) - edge detection
     * @param input Source image
     * @param output Gradient image
     * @param kernelSize Size of structuring element
     */
    static void applyMorphGradient(const cv::Mat& input, cv::Mat& output, 
                                   int kernelSize = 5);

    /**
     * @brief Top-hat transform (input - opening) - detect bright spots on dark background
     * @param input Source image
     * @param output Result image
     * @param kernelSize Size of structuring element
     */
    static void applyTopHatTransform(const cv::Mat& input, cv::Mat& output, 
                                     int kernelSize = 9);

    /**
     * @brief Black-hat transform (closing - input) - detect dark spots on bright background
     * @param input Source image
     * @param output Result image
     * @param kernelSize Size of structuring element
     */
    static void applyBlackHatTransform(const cv::Mat& input, cv::Mat& output, 
                                       int kernelSize = 9);

    // ==========================================================================
    // ADVANCED MORPHOLOGY
    // ==========================================================================

    /**
     * @brief Create custom structuring element
     * @param shape Shape type (RECT, ELLIPSE, CROSS)
     * @param size Size of structuring element
     * @return Structuring element matrix
     */
    static cv::Mat createStructuringElement(StructuringElementShape shape, 
                                           cv::Size size);

    /**
     * @brief Apply iterative morphological operation
     * @param input Source image
     * @param output Result image
     * @param operation Morphological operation type
     * @param kernelSize Size of structuring element
     * @param iterations Number of iterations
     */
    static void applyIterativeMorphology(const cv::Mat& input, cv::Mat& output,
                                        int operation, int kernelSize, 
                                        int iterations);

    // ==========================================================================
    // EDGE DETECTION SUITE
    // ==========================================================================

    /**
     * @brief Prewitt edge detection operator
     * @param input Source image (grayscale)
     * @param output Edge-detected image
     */
    static void applyPrewittOperator(const cv::Mat& input, cv::Mat& output);

    /**
     * @brief Roberts Cross edge detection operator
     * @param input Source image (grayscale)
     * @param output Edge-detected image
     */
    static void applyRobertsCross(const cv::Mat& input, cv::Mat& output);

    /**
     * @brief Laplacian of Gaussian (LoG) edge detection
     * @param input Source image
     * @param output Edge-detected image
     * @param kernelSize Size of Gaussian kernel (must be odd)
     * @param sigma Gaussian standard deviation
     */
    static void applyLoG(const cv::Mat& input, cv::Mat& output, 
                        int kernelSize = 5, double sigma = 1.0);

    /**
     * @brief Difference of Gaussians (DoG) edge detection
     * @param input Source image
     * @param output Edge-detected image
     * @param kernelSize1 First Gaussian kernel size
     * @param sigma1 First Gaussian sigma
     * @param kernelSize2 Second Gaussian kernel size
     * @param sigma2 Second Gaussian sigma
     */
    static void applyDoG(const cv::Mat& input, cv::Mat& output,
                        int kernelSize1 = 5, double sigma1 = 1.0,
                        int kernelSize2 = 9, double sigma2 = 2.0);

    /**
     * @brief Zero-crossing edge detection (Marr-Hildreth)
     * @param input Source image
     * @param output Binary edge image
     * @param kernelSize Laplacian kernel size
     */
    static void applyZeroCrossing(const cv::Mat& input, cv::Mat& output, 
                                  int kernelSize = 5);

    // ==========================================================================
    // UTILITY FUNCTIONS
    // ==========================================================================

    /**
     * @brief Check if image is valid for morphological operations
     * @param image Image to check
     * @return true if valid, false otherwise
     */
    static bool isValidImage(const cv::Mat& image);

    /**
     * @brief Normalize edge detection output to displayable range
     * @param input Edge-detected image (may have negative values)
     * @param output Normalized image (0-255)
     */
    static void normalizeEdgeImage(const cv::Mat& input, cv::Mat& output);
};

#endif // MORPHOLOGYLIB_H
