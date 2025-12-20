#include "ColorProcessingLib.h"
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <cmath>

namespace ColorProcessingLib {

// =============================================================================
// COLOR SPACE CONVERSIONS
// =============================================================================

bool convertColorSpace(const cv::Mat& input, cv::Mat& output, ColorSpace targetSpace) {
    if (!isValidImage(input)) {
        return false;
    }

    try {
        switch (targetSpace) {
            case ColorSpace::RGB:
                if (input.channels() == 3) {
                    cv::cvtColor(input, output, cv::COLOR_BGR2RGB);
                } else {
                    input.copyTo(output);
                }
                break;
            
            case ColorSpace::BGR:
                input.copyTo(output);
                break;
            
            case ColorSpace::HSV:
                rgbToHSV(input, output);
                break;
            
            case ColorSpace::HSL:
                rgbToHSL(input, output);
                break;
            
            case ColorSpace::LAB:
                rgbToLAB(input, output);
                break;
            
            case ColorSpace::YCrCb:
                rgbToYCrCb(input, output);
                break;
            
            case ColorSpace::XYZ:
                rgbToXYZ(input, output);
                break;
            
            case ColorSpace::GRAY:
                if (input.channels() == 3) {
                    cv::cvtColor(input, output, cv::COLOR_BGR2GRAY);
                } else {
                    input.copyTo(output);
                }
                break;
            
            default:
                return false;
        }
        return true;
    } catch (const cv::Exception& e) {
        return false;
    }
}

void rgbToHSV(const cv::Mat& input, cv::Mat& output) {
    if (input.empty() || input.channels() != 3) {
        return;
    }
    cv::cvtColor(input, output, cv::COLOR_BGR2HSV);
}

void rgbToLAB(const cv::Mat& input, cv::Mat& output) {
    if (input.empty() || input.channels() != 3) {
        return;
    }
    cv::cvtColor(input, output, cv::COLOR_BGR2Lab);
}

void rgbToYCrCb(const cv::Mat& input, cv::Mat& output) {
    if (input.empty() || input.channels() != 3) {
        return;
    }
    cv::cvtColor(input, output, cv::COLOR_BGR2YCrCb);
}

void rgbToHSL(const cv::Mat& input, cv::Mat& output) {
    if (input.empty() || input.channels() != 3) {
        return;
    }
    cv::cvtColor(input, output, cv::COLOR_BGR2HLS);
}

void rgbToXYZ(const cv::Mat& input, cv::Mat& output) {
    if (input.empty() || input.channels() != 3) {
        return;
    }
    cv::cvtColor(input, output, cv::COLOR_BGR2XYZ);
}

// =============================================================================
// CHANNEL OPERATIONS
// =============================================================================

void splitChannels(const cv::Mat& input, std::vector<cv::Mat>& channels) {
    if (input.empty()) {
        return;
    }
    cv::split(input, channels);
}

void mergeChannels(const std::vector<cv::Mat>& channels, cv::Mat& output) {
    if (channels.empty()) {
        return;
    }
    cv::merge(channels, output);
}

void extractChannel(const cv::Mat& input, cv::Mat& output, int channelIndex) {
    if (input.empty() || channelIndex < 0 || channelIndex >= input.channels()) {
        return;
    }
    
    std::vector<cv::Mat> channels;
    cv::split(input, channels);
    output = channels[channelIndex].clone();
}

void visualizeChannel(const cv::Mat& input, cv::Mat& output, int channelIndex) {
    if (input.empty() || channelIndex < 0 || channelIndex >= input.channels()) {
        return;
    }
    
    std::vector<cv::Mat> channels;
    cv::split(input, channels);
    
    // Create a 3-channel image with only the selected channel active
    cv::Mat zeros = cv::Mat::zeros(input.size(), CV_8UC1);
    std::vector<cv::Mat> displayChannels(3);
    
    for (int i = 0; i < 3; ++i) {
        if (i == channelIndex) {
            displayChannels[i] = channels[i];
        } else {
            displayChannels[i] = zeros;
        }
    }
    
    cv::merge(displayChannels, output);
}

void swapChannels(const cv::Mat& input, cv::Mat& output, int channel1, int channel2) {
    if (input.empty() || channel1 < 0 || channel2 < 0 || 
        channel1 >= input.channels() || channel2 >= input.channels()) {
        return;
    }
    
    std::vector<cv::Mat> channels;
    cv::split(input, channels);
    
    // Swap the channels
    std::swap(channels[channel1], channels[channel2]);
    
    cv::merge(channels, output);
}

// =============================================================================
// COLOR ADJUSTMENTS
// =============================================================================

void adjustBrightness(const cv::Mat& input, cv::Mat& output, int value) {
    if (input.empty()) {
        return;
    }
    
    // Clamp brightness value to valid range
    value = std::max(-100, std::min(100, value));
    
    // Add brightness value to all pixels
    input.convertTo(output, -1, 1.0, static_cast<double>(value));
}

void adjustContrast(const cv::Mat& input, cv::Mat& output, double value) {
    if (input.empty()) {
        return;
    }
    
    // Clamp contrast value to valid range
    value = std::max(0.5, std::min(3.0, value));
    
    // Apply contrast: output = input * contrast
    input.convertTo(output, -1, value, 0);
}

void adjustSaturation(const cv::Mat& input, cv::Mat& output, int value) {
    if (input.empty() || input.channels() != 3) {
        return;
    }
    
    // Clamp saturation value to valid range [0, 200]
    value = std::max(0, std::min(200, value));
    double saturationFactor = value / 100.0;
    
    // Convert to HSV
    cv::Mat hsv;
    cv::cvtColor(input, hsv, cv::COLOR_BGR2HSV);
    
    // Split channels
    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);
    
    // Adjust saturation channel
    channels[1].convertTo(channels[1], -1, saturationFactor, 0);
    
    // Merge and convert back
    cv::merge(channels, hsv);
    cv::cvtColor(hsv, output, cv::COLOR_HSV2BGR);
}

void adjustHue(const cv::Mat& input, cv::Mat& output, int degrees) {
    if (input.empty() || input.channels() != 3) {
        return;
    }
    
    // Normalize degrees to [0, 360]
    degrees = degrees % 360;
    if (degrees < 0) degrees += 360;
    
    // Convert to HSV
    cv::Mat hsv;
    cv::cvtColor(input, hsv, cv::COLOR_BGR2HSV);
    
    // Split channels
    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);
    
    // Adjust hue channel (OpenCV HSV: H is 0-180, so divide by 2)
    int hueShift = degrees / 2;
    channels[0] = channels[0] + hueShift;
    
    // Handle wraparound
    for (int i = 0; i < channels[0].rows; ++i) {
        for (int j = 0; j < channels[0].cols; ++j) {
            int hue = channels[0].at<uchar>(i, j);
            if (hue >= 180) hue -= 180;
            if (hue < 0) hue += 180;
            channels[0].at<uchar>(i, j) = static_cast<uchar>(hue);
        }
    }
    
    // Merge and convert back
    cv::merge(channels, hsv);
    cv::cvtColor(hsv, output, cv::COLOR_HSV2BGR);
}

void whiteBalance(const cv::Mat& input, cv::Mat& output) {
    if (input.empty() || input.channels() != 3) {
        return;
    }
    
    // Simple gray world white balance algorithm
    std::vector<cv::Mat> channels;
    cv::split(input, channels);
    
    // Calculate average for each channel
    cv::Scalar avgB = cv::mean(channels[0]);
    cv::Scalar avgG = cv::mean(channels[1]);
    cv::Scalar avgR = cv::mean(channels[2]);
    
    // Calculate gray average
    double avgGray = (avgB[0] + avgG[0] + avgR[0]) / 3.0;
    
    // Calculate scaling factors
    double scaleB = avgGray / avgB[0];
    double scaleG = avgGray / avgG[0];
    double scaleR = avgGray / avgR[0];
    
    // Apply white balance
    channels[0].convertTo(channels[0], -1, scaleB, 0);
    channels[1].convertTo(channels[1], -1, scaleG, 0);
    channels[2].convertTo(channels[2], -1, scaleR, 0);
    
    cv::merge(channels, output);
}

void adjustTemperature(const cv::Mat& input, cv::Mat& output, int temperature) {
    if (input.empty() || input.channels() != 3) {
        return;
    }
    
    // Clamp temperature to valid range [-100, 100]
    temperature = std::max(-100, std::min(100, temperature));
    
    cv::Mat result = input.clone();
    std::vector<cv::Mat> channels;
    cv::split(result, channels);
    
    if (temperature > 0) {
        // Warm: increase red, decrease blue
        double factor = temperature / 100.0;
        channels[2].convertTo(channels[2], -1, 1.0 + factor * 0.3, 0);  // Red
        channels[0].convertTo(channels[0], -1, 1.0 - factor * 0.2, 0);  // Blue
    } else if (temperature < 0) {
        // Cool: increase blue, decrease red
        double factor = -temperature / 100.0;
        channels[0].convertTo(channels[0], -1, 1.0 + factor * 0.3, 0);  // Blue
        channels[2].convertTo(channels[2], -1, 1.0 - factor * 0.2, 0);  // Red
    }
    
    cv::merge(channels, output);
}

void adjustColors(const cv::Mat& input, cv::Mat& output,
                 int brightness, double contrast, int saturation, int hue) {
    if (input.empty()) {
        return;
    }
    
    cv::Mat temp1, temp2, temp3;
    
    // Apply adjustments in sequence
    adjustBrightness(input, temp1, brightness);
    adjustContrast(temp1, temp2, contrast);
    adjustSaturation(temp2, temp3, saturation);
    adjustHue(temp3, output, hue);
}

// =============================================================================
// COLOR GRADING & EFFECTS
// =============================================================================

void applySepiaEffect(const cv::Mat& input, cv::Mat& output, double intensity) {
    if (input.empty() || input.channels() != 3) {
        return;
    }
    
    intensity = std::max(0.0, std::min(1.0, intensity));
    
    // Sepia transformation matrix
    cv::Mat kernel = (cv::Mat_<float>(3, 3) <<
        0.272, 0.534, 0.131,
        0.349, 0.686, 0.168,
        0.393, 0.769, 0.189);
    
    cv::Mat floatInput;
    input.convertTo(floatInput, CV_32F);
    
    cv::Mat sepia;
    cv::transform(floatInput, sepia, kernel);
    
    // Blend with original based on intensity
    cv::Mat result;
    cv::addWeighted(floatInput, 1.0 - intensity, sepia, intensity, 0, result);
    
    result.convertTo(output, CV_8U);
}

void applyCoolFilter(const cv::Mat& input, cv::Mat& output, double intensity) {
    if (input.empty() || input.channels() != 3) {
        return;
    }
    
    intensity = std::max(0.0, std::min(1.0, intensity));
    
    cv::Mat result = input.clone();
    std::vector<cv::Mat> channels;
    cv::split(result, channels);
    
    // Increase blue, slightly decrease red
    channels[0].convertTo(channels[0], -1, 1.0 + intensity * 0.2, 0);  // Blue
    channels[2].convertTo(channels[2], -1, 1.0 - intensity * 0.1, 0);  // Red
    
    cv::merge(channels, output);
}

void applyWarmFilter(const cv::Mat& input, cv::Mat& output, double intensity) {
    if (input.empty() || input.channels() != 3) {
        return;
    }
    
    intensity = std::max(0.0, std::min(1.0, intensity));
    
    cv::Mat result = input.clone();
    std::vector<cv::Mat> channels;
    cv::split(result, channels);
    
    // Increase red and green (yellow), decrease blue
    channels[1].convertTo(channels[1], -1, 1.0 + intensity * 0.1, 0);  // Green
    channels[2].convertTo(channels[2], -1, 1.0 + intensity * 0.2, 0);  // Red
    channels[0].convertTo(channels[0], -1, 1.0 - intensity * 0.1, 0);  // Blue
    
    cv::merge(channels, output);
}

void applyVintageEffect(const cv::Mat& input, cv::Mat& output) {
    if (input.empty() || input.channels() != 3) {
        return;
    }
    
    // Apply sepia first
    cv::Mat sepia;
    applySepiaEffect(input, sepia, 0.8);
    
    // Reduce contrast slightly
    cv::Mat contrast;
    adjustContrast(sepia, contrast, 0.85);
    
    // Add slight vignette effect by darkening corners
    cv::Mat vignette = contrast.clone();
    int cx = vignette.cols / 2;
    int cy = vignette.rows / 2;
    double maxDist = std::sqrt(cx * cx + cy * cy);
    
    for (int y = 0; y < vignette.rows; ++y) {
        for (int x = 0; x < vignette.cols; ++x) {
            double dist = std::sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
            double factor = 1.0 - (dist / maxDist) * 0.5;
            
            cv::Vec3b& pixel = vignette.at<cv::Vec3b>(y, x);
            pixel[0] = clampToByte(static_cast<int>(pixel[0] * factor));
            pixel[1] = clampToByte(static_cast<int>(pixel[1] * factor));
            pixel[2] = clampToByte(static_cast<int>(pixel[2] * factor));
        }
    }
    
    output = vignette;
}

void applyLUT(const cv::Mat& input, cv::Mat& output, const cv::Mat& lut) {
    if (input.empty() || lut.empty()) {
        return;
    }
    
    cv::LUT(input, lut, output);
}

void createColorGradingLUT(cv::Mat& lut, const QString& style) {
    lut = cv::Mat(1, 256, CV_8UC3);
    
    for (int i = 0; i < 256; ++i) {
        cv::Vec3b& pixel = lut.at<cv::Vec3b>(0, i);
        
        if (style == "warm") {
            pixel[0] = clampToByte(i * 0.9);        // Blue - reduce
            pixel[1] = clampToByte(i * 1.05);       // Green - increase slightly
            pixel[2] = clampToByte(i * 1.15);       // Red - increase
        } else if (style == "cool") {
            pixel[0] = clampToByte(i * 1.15);       // Blue - increase
            pixel[1] = clampToByte(i * 1.0);        // Green - unchanged
            pixel[2] = clampToByte(i * 0.9);        // Red - reduce
        } else if (style == "vintage") {
            // Vintage with sepia-like tones and faded contrast
            double t = i / 255.0;
            pixel[0] = clampToByte(131 * t + 20);
            pixel[1] = clampToByte(168 * t + 40);
            pixel[2] = clampToByte(189 * t + 50);
        } else if (style == "dramatic") {
            // High contrast S-curve
            double t = i / 255.0;
            double curve = 1.0 / (1.0 + std::exp(-12.0 * (t - 0.5)));
            int val = static_cast<int>(curve * 255);
            pixel[0] = clampToByte(val);
            pixel[1] = clampToByte(val);
            pixel[2] = clampToByte(val);
        } else {
            // Default: no change
            pixel[0] = i;
            pixel[1] = i;
            pixel[2] = i;
        }
    }
}

// =============================================================================
// UTILITY FUNCTIONS
// =============================================================================

QString getColorSpaceName(ColorSpace space) {
    switch (space) {
        case ColorSpace::RGB:    return "RGB";
        case ColorSpace::BGR:    return "BGR";
        case ColorSpace::HSV:    return "HSV";
        case ColorSpace::HSL:    return "HSL";
        case ColorSpace::LAB:    return "LAB";
        case ColorSpace::YCrCb:  return "YCrCb";
        case ColorSpace::XYZ:    return "XYZ";
        case ColorSpace::GRAY:   return "Grayscale";
        default:                 return "Unknown";
    }
}

bool isValidImage(const cv::Mat& image) {
    return !image.empty() && (image.channels() == 1 || image.channels() == 3 || image.channels() == 4);
}

void normalizeChannelForVisualization(const cv::Mat& input, cv::Mat& output) {
    if (input.empty()) {
        return;
    }
    
    double minVal, maxVal;
    cv::minMaxLoc(input, &minVal, &maxVal);
    
    if (maxVal - minVal > 0) {
        input.convertTo(output, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));
    } else {
        input.convertTo(output, CV_8U);
    }
}

} // namespace ColorProcessingLib
