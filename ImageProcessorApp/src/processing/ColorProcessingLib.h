#ifndef COLORPROCESSINGLIB_H
#define COLORPROCESSINGLIB_H

#include <opencv2/opencv.hpp>
#include <QString>
#include <vector>

/**
 * @brief ColorProcessingLib provides comprehensive color space operations and channel manipulation
 * 
 * This library implements Lab 8 functionality including:
 * - Color space conversions (RGB, HSV, LAB, YCrCb, HSL, XYZ)
 * - Channel operations (split, merge, manipulation)
 * - Color adjustments (brightness, contrast, saturation, hue)
 * - Color grading effects (sepia, cool/warm filters, vintage)
 * 
 * All functions use OpenCV C++ API for image processing
 */
namespace ColorProcessingLib {

    /**
     * @brief Supported color spaces for conversion
     */
    enum class ColorSpace {
        RGB,      // Red, Green, Blue
        BGR,      // Blue, Green, Red (OpenCV default)
        HSV,      // Hue, Saturation, Value
        HSL,      // Hue, Saturation, Lightness
        LAB,      // L*a*b* (perceptually uniform)
        YCrCb,    // Luminance, Chroma Red, Chroma Blue
        XYZ,      // CIE XYZ color space
        GRAY      // Grayscale
    };

    /**
     * @brief Channel indices for RGB/BGR images
     */
    enum class Channel {
        RED = 0,
        GREEN = 1,
        BLUE = 2,
        ALPHA = 3
    };

    // =============================================================================
    // COLOR SPACE CONVERSIONS
    // =============================================================================

    /**
     * @brief Convert image to specified color space
     * @param input Input image (should be BGR format from OpenCV)
     * @param output Converted image in target color space
     * @param targetSpace Target color space
     * @return true if conversion successful, false otherwise
     */
    bool convertColorSpace(const cv::Mat& input, cv::Mat& output, ColorSpace targetSpace);

    /**
     * @brief Convert image from RGB to HSV color space
     * @param input Input BGR image
     * @param output Output HSV image (H: 0-180, S: 0-255, V: 0-255)
     */
    void rgbToHSV(const cv::Mat& input, cv::Mat& output);

    /**
     * @brief Convert image from RGB to LAB color space
     * @param input Input BGR image
     * @param output Output LAB image (L: 0-100, a: -127 to 127, b: -127 to 127)
     */
    void rgbToLAB(const cv::Mat& input, cv::Mat& output);

    /**
     * @brief Convert image from RGB to YCrCb color space
     * @param input Input BGR image
     * @param output Output YCrCb image
     */
    void rgbToYCrCb(const cv::Mat& input, cv::Mat& output);

    /**
     * @brief Convert image from RGB to HSL color space
     * @param input Input BGR image
     * @param output Output HSL image
     */
    void rgbToHSL(const cv::Mat& input, cv::Mat& output);

    /**
     * @brief Convert image from RGB to XYZ color space
     * @param input Input BGR image
     * @param output Output XYZ image
     */
    void rgbToXYZ(const cv::Mat& input, cv::Mat& output);

    // =============================================================================
    // CHANNEL OPERATIONS
    // =============================================================================

    /**
     * @brief Split image into separate channel images
     * @param input Input multi-channel image
     * @param channels Vector of output single-channel images
     */
    void splitChannels(const cv::Mat& input, std::vector<cv::Mat>& channels);

    /**
     * @brief Merge separate channel images into single image
     * @param channels Vector of input single-channel images
     * @param output Output multi-channel image
     */
    void mergeChannels(const std::vector<cv::Mat>& channels, cv::Mat& output);

    /**
     * @brief Extract single channel from image
     * @param input Input multi-channel image
     * @param output Output single-channel image
     * @param channelIndex Index of channel to extract (0=B, 1=G, 2=R for BGR)
     */
    void extractChannel(const cv::Mat& input, cv::Mat& output, int channelIndex);

    /**
     * @brief Visualize single channel as grayscale image
     * @param input Input multi-channel image
     * @param output Output grayscale visualization
     * @param channelIndex Index of channel to visualize
     */
    void visualizeChannel(const cv::Mat& input, cv::Mat& output, int channelIndex);

    /**
     * @brief Swap two channels in an image
     * @param input Input image
     * @param output Output image with swapped channels
     * @param channel1 First channel index
     * @param channel2 Second channel index
     */
    void swapChannels(const cv::Mat& input, cv::Mat& output, int channel1, int channel2);

    // =============================================================================
    // COLOR ADJUSTMENTS
    // =============================================================================

    /**
     * @brief Adjust image brightness
     * @param input Input image
     * @param output Output image with adjusted brightness
     * @param value Brightness adjustment value (-100 to +100)
     */
    void adjustBrightness(const cv::Mat& input, cv::Mat& output, int value);

    /**
     * @brief Adjust image contrast
     * @param input Input image
     * @param output Output image with adjusted contrast
     * @param value Contrast multiplier (0.5 to 3.0, 1.0 = no change)
     */
    void adjustContrast(const cv::Mat& input, cv::Mat& output, double value);

    /**
     * @brief Adjust image saturation
     * @param input Input BGR image
     * @param output Output image with adjusted saturation
     * @param value Saturation percentage (0 to 200, 100 = no change)
     */
    void adjustSaturation(const cv::Mat& input, cv::Mat& output, int value);

    /**
     * @brief Shift hue in image
     * @param input Input BGR image
     * @param output Output image with shifted hue
     * @param degrees Hue shift in degrees (0-360)
     */
    void adjustHue(const cv::Mat& input, cv::Mat& output, int degrees);

    /**
     * @brief Apply white balance correction
     * @param input Input BGR image
     * @param output Output white balanced image
     */
    void whiteBalance(const cv::Mat& input, cv::Mat& output);

    /**
     * @brief Adjust color temperature
     * @param input Input BGR image
     * @param output Output image with adjusted temperature
     * @param temperature Temperature value (-100 = cool, 0 = neutral, +100 = warm)
     */
    void adjustTemperature(const cv::Mat& input, cv::Mat& output, int temperature);

    /**
     * @brief Apply all color adjustments at once
     * @param input Input image
     * @param output Output adjusted image
     * @param brightness Brightness value (-100 to +100)
     * @param contrast Contrast multiplier (0.5 to 3.0)
     * @param saturation Saturation percentage (0 to 200)
     * @param hue Hue shift in degrees (0-360)
     */
    void adjustColors(const cv::Mat& input, cv::Mat& output,
                     int brightness, double contrast, int saturation, int hue);

    // =============================================================================
    // COLOR GRADING & EFFECTS
    // =============================================================================

    /**
     * @brief Apply sepia tone effect
     * @param input Input BGR image
     * @param output Output sepia-toned image
     * @param intensity Effect intensity (0.0 to 1.0)
     */
    void applySepiaEffect(const cv::Mat& input, cv::Mat& output, double intensity = 1.0);

    /**
     * @brief Apply cool filter (blue tint)
     * @param input Input BGR image
     * @param output Output cooled image
     * @param intensity Effect intensity (0.0 to 1.0)
     */
    void applyCoolFilter(const cv::Mat& input, cv::Mat& output, double intensity = 0.5);

    /**
     * @brief Apply warm filter (orange/yellow tint)
     * @param input Input BGR image
     * @param output Output warmed image
     * @param intensity Effect intensity (0.0 to 1.0)
     */
    void applyWarmFilter(const cv::Mat& input, cv::Mat& output, double intensity = 0.5);

    /**
     * @brief Apply vintage/retro effect
     * @param input Input BGR image
     * @param output Output vintage-styled image
     */
    void applyVintageEffect(const cv::Mat& input, cv::Mat& output);

    /**
     * @brief Apply custom color grading using LUT
     * @param input Input BGR image
     * @param output Output graded image
     * @param lut Look-Up Table (256x1x3 matrix)
     */
    void applyLUT(const cv::Mat& input, cv::Mat& output, const cv::Mat& lut);

    /**
     * @brief Create LUT for specific color grading
     * @param lut Output LUT matrix (256x1x3)
     * @param style Grading style ("warm", "cool", "vintage", "dramatic")
     */
    void createColorGradingLUT(cv::Mat& lut, const QString& style);

    // =============================================================================
    // UTILITY FUNCTIONS
    // =============================================================================

    /**
     * @brief Get color space name as string
     * @param space ColorSpace enum value
     * @return String representation of color space
     */
    QString getColorSpaceName(ColorSpace space);

    /**
     * @brief Check if image is valid for color processing
     * @param image Input image to validate
     * @return true if valid, false otherwise
     */
    bool isValidImage(const cv::Mat& image);

    /**
     * @brief Clamp value to byte range [0, 255]
     * @param value Input value
     * @return Clamped value
     */
    inline uchar clampToByte(int value) {
        return static_cast<uchar>(std::max(0, std::min(255, value)));
    }

    /**
     * @brief Normalize channel values to [0, 255] for visualization
     * @param input Input channel (may have different range)
     * @param output Output normalized channel
     */
    void normalizeChannelForVisualization(const cv::Mat& input, cv::Mat& output);

} // namespace ColorProcessingLib

#endif // COLORPROCESSINGLIB_H
