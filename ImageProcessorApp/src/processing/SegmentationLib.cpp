#include "SegmentationLib.h"
#include <algorithm>
#include <cmath>
#include <random>

// =============================================================================
// THRESHOLDING VARIANTS
// =============================================================================

void SegmentationLib::applyAdaptiveThreshold(const cv::Mat& input, cv::Mat& output,
                                             double maxValue, int method,
                                             int blockSize, double C) {
    if (!isValidImage(input)) {
        output = input.clone();
        return;
    }
    
    bool wasColor = (input.channels() == 3);
    
    // Convert to grayscale if needed
    cv::Mat gray;
    convertToGrayscale(input, gray);
    
    // Ensure odd block size
    if (blockSize % 2 == 0) blockSize++;
    blockSize = std::max(3, std::min(99, blockSize));
    
    cv::Mat binary;
    cv::adaptiveThreshold(gray, binary, maxValue, method, 
                         cv::THRESH_BINARY, blockSize, C);
    
    // Convert back to color if input was color
    if (wasColor) {
        cv::cvtColor(binary, output, cv::COLOR_GRAY2BGR);
    } else {
        output = binary;
    }
}

void SegmentationLib::applyMultiLevelThreshold(const cv::Mat& input, cv::Mat& output,
                                               int levels) {
    if (!isValidImage(input)) {
        output = input.clone();
        return;
    }
    
    bool wasColor = (input.channels() == 3);
    
    // Convert to grayscale if needed
    cv::Mat gray;
    convertToGrayscale(input, gray);
    
    levels = std::max(2, std::min(5, levels));
    
    // Calculate histogram
    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange = {range};
    cv::Mat hist;
    cv::calcHist(&gray, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange);
    
    // Find threshold values by dividing histogram into equal parts
    std::vector<int> thresholds;
    float totalPixels = gray.rows * gray.cols;
    float pixelsPerLevel = totalPixels / levels;
    
    float cumSum = 0;
    int currentLevel = 1;
    
    for (int i = 0; i < 256 && currentLevel < levels; ++i) {
        cumSum += hist.at<float>(i);
        if (cumSum >= pixelsPerLevel * currentLevel) {
            thresholds.push_back(i);
            currentLevel++;
        }
    }
    
    // Apply multi-level thresholding
    cv::Mat result = cv::Mat::zeros(gray.size(), CV_8U);
    for (int i = 0; i < gray.rows; ++i) {
        for (int j = 0; j < gray.cols; ++j) {
            uchar pixel = gray.at<uchar>(i, j);
            uchar level = 0;
            for (size_t t = 0; t < thresholds.size(); ++t) {
                if (pixel >= thresholds[t]) {
                    level = (255 / levels) * (t + 1);
                }
            }
            result.at<uchar>(i, j) = level;
        }
    }
    
    // Convert back to color if input was color
    if (wasColor) {
        cv::cvtColor(result, output, cv::COLOR_GRAY2BGR);
    } else {
        output = result;
    }
}

// =============================================================================
// WATERSHED SEGMENTATION
// =============================================================================

void SegmentationLib::createWatershedMarkers(const cv::Mat& input, cv::Mat& markers,
                                             double distThreshold) {
    // Convert to grayscale
    cv::Mat gray;
    convertToGrayscale(input, gray);
    
    // Threshold to binary
    cv::Mat binary;
    cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    
    // Noise removal with morphology
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    cv::morphologyEx(binary, binary, cv::MORPH_OPEN, kernel, cv::Point(-1, -1), 2);
    
    // Sure background area
    cv::Mat sureBg;
    cv::dilate(binary, sureBg, kernel, cv::Point(-1, -1), 3);
    
    // Distance transform
    cv::Mat distTransform;
    cv::distanceTransform(binary, distTransform, cv::DIST_L2, 5);
    
    // Sure foreground area
    cv::Mat sureFg;
    double maxDist;
    cv::minMaxLoc(distTransform, nullptr, &maxDist);
    cv::threshold(distTransform, sureFg, distThreshold * maxDist, 255, cv::THRESH_BINARY);
    sureFg.convertTo(sureFg, CV_8U);
    
    // Unknown region
    cv::Mat unknown;
    cv::subtract(sureBg, sureFg, unknown);
    
    // Label markers
    markers = cv::Mat::zeros(sureFg.size(), CV_32S);
    cv::connectedComponents(sureFg, markers);
    
    // Add one to all labels so background is not 0, but 1
    markers = markers + 1;
    
    // Mark unknown region as 0
    for (int i = 0; i < markers.rows; ++i) {
        for (int j = 0; j < markers.cols; ++j) {
            if (unknown.at<uchar>(i, j) == 255) {
                markers.at<int>(i, j) = 0;
            }
        }
    }
}

void SegmentationLib::applyWatershedAuto(const cv::Mat& input, cv::Mat& output,
                                         double distThreshold) {
    if (!isValidImage(input)) {
        output = input.clone();
        return;
    }
    
    // Ensure input is color
    cv::Mat colorInput;
    if (input.channels() == 1) {
        cv::cvtColor(input, colorInput, cv::COLOR_GRAY2BGR);
    } else {
        colorInput = input.clone();
    }
    
    // Create automatic markers
    cv::Mat markers;
    createWatershedMarkers(colorInput, markers, distThreshold);
    
    // Apply watershed
    cv::watershed(colorInput, markers);
    
    // Colorize result
    colorizeLabels(markers, output);
    
    // Draw boundaries
    for (int i = 0; i < markers.rows; ++i) {
        for (int j = 0; j < markers.cols; ++j) {
            if (markers.at<int>(i, j) == -1) {
                output.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 255); // Red boundaries
            }
        }
    }
}

void SegmentationLib::applyWatershedManual(const cv::Mat& input, cv::Mat& markers,
                                           cv::Mat& output) {
    if (!isValidImage(input) || markers.empty()) {
        output = input.clone();
        return;
    }
    
    // Ensure input is color
    cv::Mat colorInput;
    if (input.channels() == 1) {
        cv::cvtColor(input, colorInput, cv::COLOR_GRAY2BGR);
    } else {
        colorInput = input.clone();
    }
    
    // Apply watershed
    cv::watershed(colorInput, markers);
    
    // Colorize result
    colorizeLabels(markers, output);
    
    // Draw boundaries
    for (int i = 0; i < markers.rows; ++i) {
        for (int j = 0; j < markers.cols; ++j) {
            if (markers.at<int>(i, j) == -1) {
                output.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 255);
            }
        }
    }
}

// =============================================================================
// GRABCUT SEGMENTATION
// =============================================================================

void SegmentationLib::applyGrabCut(const cv::Mat& input, cv::Mat& output,
                                   const cv::Rect& rect, int iterations) {
    if (!isValidImage(input) || input.channels() != 3) {
        output = input.clone();
        return;
    }
    
    cv::Mat mask = cv::Mat::zeros(input.size(), CV_8U);
    cv::Mat bgModel, fgModel;
    
    try {
        cv::grabCut(input, mask, rect, bgModel, fgModel, iterations, cv::GC_INIT_WITH_RECT);
        
        // Create binary mask (0 or 255) - always as color to match input
        output = cv::Mat::zeros(input.size(), CV_8UC3);
        
        for (int i = 0; i < mask.rows; ++i) {
            for (int j = 0; j < mask.cols; ++j) {
                if (mask.at<uchar>(i, j) == cv::GC_FGD || 
                    mask.at<uchar>(i, j) == cv::GC_PR_FGD) {
                    // Set all channels to 255 (white foreground)
                    output.at<cv::Vec3b>(i, j) = cv::Vec3b(255, 255, 255);
                }
            }
        }
    } catch (const cv::Exception& e) {
        output = cv::Mat::zeros(input.size(), CV_8UC3);
    }
}

void SegmentationLib::applyGrabCutWithMask(const cv::Mat& input, cv::Mat& mask,
                                           int iterations) {
    if (!isValidImage(input) || input.channels() != 3 || mask.empty()) {
        return;
    }
    
    cv::Mat bgModel, fgModel;
    
    try {
        cv::grabCut(input, mask, cv::Rect(), bgModel, fgModel, 
                   iterations, cv::GC_INIT_WITH_MASK);
    } catch (const cv::Exception& e) {
        // Handle error silently
    }
}

// =============================================================================
// CONTOUR DETECTION & ANALYSIS
// =============================================================================

void SegmentationLib::findAllContours(const cv::Mat& input,
                                      std::vector<std::vector<cv::Point>>& contours,
                                      std::vector<cv::Vec4i>& hierarchy,
                                      int mode, int method) {
    if (!isValidImage(input)) {
        contours.clear();
        hierarchy.clear();
        return;
    }
    
    // Ensure binary image
    cv::Mat binary;
    if (input.type() != CV_8U) {
        convertToGrayscale(input, binary);
        cv::threshold(binary, binary, 127, 255, cv::THRESH_BINARY);
    } else {
        binary = input.clone();
    }
    
    cv::findContours(binary, contours, hierarchy, mode, method);
}

SegmentationLib::ContourProperties SegmentationLib::calculateContourProperties(
    const std::vector<cv::Point>& contour) {
    
    ContourProperties props;
    props.contour = contour;
    
    // Area
    props.area = cv::contourArea(contour);
    
    // Perimeter
    props.perimeter = cv::arcLength(contour, true);
    
    // Centroid using moments
    cv::Moments m = cv::moments(contour);
    if (m.m00 != 0) {
        props.centroid = cv::Point2f(static_cast<float>(m.m10 / m.m00),
                                     static_cast<float>(m.m01 / m.m00));
    } else {
        props.centroid = cv::Point2f(0, 0);
    }
    
    // Bounding box
    props.boundingBox = cv::boundingRect(contour);
    
    // Circularity (4π×area/perimeter²)
    if (props.perimeter > 0) {
        props.circularity = (4 * CV_PI * props.area) / (props.perimeter * props.perimeter);
    } else {
        props.circularity = 0;
    }
    
    return props;
}

std::vector<SegmentationLib::ContourProperties> 
SegmentationLib::calculateAllContourProperties(
    const std::vector<std::vector<cv::Point>>& contours) {
    
    std::vector<ContourProperties> allProps;
    allProps.reserve(contours.size());
    
    for (const auto& contour : contours) {
        allProps.push_back(calculateContourProperties(contour));
    }
    
    return allProps;
}

void SegmentationLib::drawContoursWithInfo(cv::Mat& image,
                                           const std::vector<std::vector<cv::Point>>& contours,
                                           const cv::Scalar& color,
                                           int thickness,
                                           bool drawBoundingBoxes,
                                           bool drawCentroids) {
    if (image.empty() || contours.empty()) {
        return;
    }
    
    // Draw all contours
    cv::drawContours(image, contours, -1, color, thickness);
    
    if (drawBoundingBoxes || drawCentroids) {
        for (size_t i = 0; i < contours.size(); ++i) {
            ContourProperties props = calculateContourProperties(contours[i]);
            
            // Draw bounding box
            if (drawBoundingBoxes) {
                cv::rectangle(image, props.boundingBox, cv::Scalar(255, 0, 0), 2);
            }
            
            // Draw centroid
            if (drawCentroids) {
                cv::circle(image, props.centroid, 5, cv::Scalar(0, 255, 255), -1);
            }
        }
    }
}

void SegmentationLib::filterContoursByArea(
    const std::vector<std::vector<cv::Point>>& contours,
    std::vector<std::vector<cv::Point>>& filtered,
    double minArea, double maxArea) {
    
    filtered.clear();
    
    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);
        if (area >= minArea && (maxArea < 0 || area <= maxArea)) {
            filtered.push_back(contour);
        }
    }
}

void SegmentationLib::filterContoursByCircularity(
    const std::vector<std::vector<cv::Point>>& contours,
    std::vector<std::vector<cv::Point>>& filtered,
    double minCircularity, double maxCircularity) {
    
    filtered.clear();
    
    for (const auto& contour : contours) {
        ContourProperties props = calculateContourProperties(contour);
        if (props.circularity >= minCircularity && props.circularity <= maxCircularity) {
            filtered.push_back(contour);
        }
    }
}

// =============================================================================
// UTILITY FUNCTIONS
// =============================================================================

void SegmentationLib::convertToGrayscale(const cv::Mat& input, cv::Mat& output) {
    if (input.channels() == 3) {
        cv::cvtColor(input, output, cv::COLOR_BGR2GRAY);
    } else {
        output = input.clone();
    }
}

bool SegmentationLib::isValidImage(const cv::Mat& image) {
    return !image.empty() && (image.channels() == 1 || image.channels() == 3);
}

void SegmentationLib::colorizeLabels(const cv::Mat& labels, cv::Mat& output) {
    if (labels.empty()) {
        output = cv::Mat();
        return;
    }
    
    // Find number of unique labels
    double minVal, maxVal;
    cv::minMaxLoc(labels, &minVal, &maxVal);
    int numLabels = static_cast<int>(maxVal) + 1;
    
    // Generate random colors for each label
    std::vector<cv::Vec3b> colors(numLabels);
    std::mt19937 rng(42); // Fixed seed for reproducibility
    std::uniform_int_distribution<int> dist(0, 255);
    
    colors[0] = cv::Vec3b(0, 0, 0); // Background is black
    for (int i = 1; i < numLabels; ++i) {
        colors[i] = cv::Vec3b(dist(rng), dist(rng), dist(rng));
    }
    
    // Create output image
    output = cv::Mat::zeros(labels.size(), CV_8UC3);
    for (int i = 0; i < labels.rows; ++i) {
        for (int j = 0; j < labels.cols; ++j) {
            int label = labels.at<int>(i, j);
            if (label >= 0 && label < numLabels) {
                output.at<cv::Vec3b>(i, j) = colors[label];
            }
        }
    }
}
