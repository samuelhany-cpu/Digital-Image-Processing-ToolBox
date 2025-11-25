#include "ImageProcessingLib.h"

namespace ImageProcessingLib {

void convertToGrayscale(const cv::Mat& input, cv::Mat& output) {
    if (input.channels() == 3) {
        cv::cvtColor(input, output, cv::COLOR_BGR2GRAY);
    } else {
        output = input.clone();
    }
}

void applyBinaryThreshold(const cv::Mat& input, cv::Mat& output, int threshold) {
    cv::Mat gray;
    if (input.channels() == 3) {
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = input.clone();
    }
    
    cv::threshold(gray, output, threshold, 255, cv::THRESH_BINARY);
}

void applyGaussianBlur(const cv::Mat& input, cv::Mat& output, int kernelSize) {
    cv::GaussianBlur(input, output, cv::Size(kernelSize, kernelSize), 0);
}

void applyEdgeDetection(const cv::Mat& input, cv::Mat& output, 
                       int lowThreshold, int highThreshold) {
    cv::Mat gray;
    if (input.channels() == 3) {
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = input.clone();
    }
    
    cv::Canny(gray, output, lowThreshold, highThreshold);
}

void invertColors(const cv::Mat& input, cv::Mat& output) {
    output = 255 - input;
}

void applyHistogramEqualization(const cv::Mat& input, cv::Mat& output) {
    if (input.channels() == 3) {
        // Convert to YCrCb for color images
        cv::Mat ycrcb;
        cv::cvtColor(input, ycrcb, cv::COLOR_BGR2YCrCb);
        
        std::vector<cv::Mat> channels;
        cv::split(ycrcb, channels);
        cv::equalizeHist(channels[0], channels[0]);
        cv::merge(channels, ycrcb);
        
        cv::cvtColor(ycrcb, output, cv::COLOR_YCrCb2BGR);
    } else {
        cv::equalizeHist(input, output);
    }
}

void applyOtsuThresholding(const cv::Mat& input, cv::Mat& output) {
    cv::Mat gray;
    if (input.channels() == 3) {
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = input.clone();
    }
    
    cv::threshold(gray, output, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
}

} // namespace ImageProcessingLib
