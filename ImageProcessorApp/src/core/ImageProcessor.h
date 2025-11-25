#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <opencv2/opencv.hpp>
#include <QString>

// Theme namespace for consistent styling
namespace Theme {
    // Main backgrounds
    constexpr const char* BG_PRIMARY = "#0a0e27";
    constexpr const char* BG_SECONDARY = "#0f1535";
    constexpr const char* BG_TERTIARY = "#1a2351";
    constexpr const char* BG_QUATERNARY = "#252d48";
    
    // Text colors
    constexpr const char* FG_PRIMARY = "#f8f9fc";
    constexpr const char* FG_SECONDARY = "#9ca3b3";
    constexpr const char* FG_TERTIARY = "#7a8399";
    
    // Accent colors
    constexpr const char* ACCENT_CYAN = "#00d4ff";
    constexpr const char* ACCENT_CYAN_HOVER = "#00e8ff";
    constexpr const char* ACCENT_PINK = "#ff006e";
    constexpr const char* ACCENT_LIME = "#39ff14";
    constexpr const char* SUCCESS_GREEN = "#1fa65a";
    constexpr const char* SUCCESS_GREEN_LIGHT = "#2e8b57";
    
    // Status colors
    constexpr const char* INFO_COLOR = "#00d4ff";
    constexpr const char* WARNING_COLOR = "#ffaa00";
    constexpr const char* ERROR_COLOR = "#ff006e";
    
    // UI elements
    constexpr const char* BORDER_COLOR = "#1f2845";
    constexpr const char* BORDER_LIGHT = "#3a4a6f";
}

// Utility class for image processing operations
class ImageProcessor {
public:
    static cv::Mat applyTranslation(const cv::Mat& image, int tx, int ty);
    static cv::Mat applyRotation(const cv::Mat& image, double angle, cv::Point2f center = cv::Point2f(-1, -1));
    static cv::Mat applyZoom(const cv::Mat& image, double factor);
    static cv::Mat applySkew(const cv::Mat& image, double skewX = 0.5);
    static cv::Mat calculateHistogram(const cv::Mat& image);
    static void getImageStats(const cv::Mat& image, double& minVal, double& maxVal, double& meanVal, double& stdDev);
};

#endif // IMAGEPROCESSOR_H