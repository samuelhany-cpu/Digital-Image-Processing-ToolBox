#include "MorphologyLib.h"
#include <algorithm>

// =============================================================================
// BASIC MORPHOLOGICAL OPERATIONS
// =============================================================================

void MorphologyLib::applyErosion(const cv::Mat& input, cv::Mat& output, 
                                 int kernelSize, StructuringElementShape shape,
                                 int iterations) {
    if (!isValidImage(input)) {
        output = input.clone();
        return;
    }
    
    // Ensure odd kernel size
    if (kernelSize % 2 == 0) kernelSize++;
    kernelSize = std::max(3, std::min(21, kernelSize));
    
    cv::Mat kernel = createStructuringElement(shape, cv::Size(kernelSize, kernelSize));
    cv::erode(input, output, kernel, cv::Point(-1, -1), iterations);
}

void MorphologyLib::applyDilation(const cv::Mat& input, cv::Mat& output, 
                                  int kernelSize, StructuringElementShape shape,
                                  int iterations) {
    if (!isValidImage(input)) {
        output = input.clone();
        return;
    }
    
    // Ensure odd kernel size
    if (kernelSize % 2 == 0) kernelSize++;
    kernelSize = std::max(3, std::min(21, kernelSize));
    
    cv::Mat kernel = createStructuringElement(shape, cv::Size(kernelSize, kernelSize));
    cv::dilate(input, output, kernel, cv::Point(-1, -1), iterations);
}

void MorphologyLib::applyOpening(const cv::Mat& input, cv::Mat& output, 
                                 int kernelSize, StructuringElementShape shape) {
    if (!isValidImage(input)) {
        output = input.clone();
        return;
    }
    
    // Ensure odd kernel size
    if (kernelSize % 2 == 0) kernelSize++;
    kernelSize = std::max(3, std::min(21, kernelSize));
    
    cv::Mat kernel = createStructuringElement(shape, cv::Size(kernelSize, kernelSize));
    cv::morphologyEx(input, output, cv::MORPH_OPEN, kernel);
}

void MorphologyLib::applyClosing(const cv::Mat& input, cv::Mat& output, 
                                 int kernelSize, StructuringElementShape shape) {
    if (!isValidImage(input)) {
        output = input.clone();
        return;
    }
    
    // Ensure odd kernel size
    if (kernelSize % 2 == 0) kernelSize++;
    kernelSize = std::max(3, std::min(21, kernelSize));
    
    cv::Mat kernel = createStructuringElement(shape, cv::Size(kernelSize, kernelSize));
    cv::morphologyEx(input, output, cv::MORPH_CLOSE, kernel);
}

void MorphologyLib::applyMorphGradient(const cv::Mat& input, cv::Mat& output, 
                                       int kernelSize) {
    if (!isValidImage(input)) {
        output = input.clone();
        return;
    }
    
    // Ensure odd kernel size
    if (kernelSize % 2 == 0) kernelSize++;
    kernelSize = std::max(3, std::min(21, kernelSize));
    
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, 
                                               cv::Size(kernelSize, kernelSize));
    cv::morphologyEx(input, output, cv::MORPH_GRADIENT, kernel);
}

void MorphologyLib::applyTopHatTransform(const cv::Mat& input, cv::Mat& output, 
                                         int kernelSize) {
    if (!isValidImage(input)) {
        output = input.clone();
        return;
    }
    
    // Ensure odd kernel size
    if (kernelSize % 2 == 0) kernelSize++;
    kernelSize = std::max(3, std::min(21, kernelSize));
    
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, 
                                               cv::Size(kernelSize, kernelSize));
    cv::morphologyEx(input, output, cv::MORPH_TOPHAT, kernel);
}

void MorphologyLib::applyBlackHatTransform(const cv::Mat& input, cv::Mat& output, 
                                           int kernelSize) {
    if (!isValidImage(input)) {
        output = input.clone();
        return;
    }
    
    // Ensure odd kernel size
    if (kernelSize % 2 == 0) kernelSize++;
    kernelSize = std::max(3, std::min(21, kernelSize));
    
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, 
                                               cv::Size(kernelSize, kernelSize));
    cv::morphologyEx(input, output, cv::MORPH_BLACKHAT, kernel);
}

// =============================================================================
// ADVANCED MORPHOLOGY
// =============================================================================

cv::Mat MorphologyLib::createStructuringElement(StructuringElementShape shape, 
                                                cv::Size size) {
    return cv::getStructuringElement(static_cast<int>(shape), size);
}

void MorphologyLib::applyIterativeMorphology(const cv::Mat& input, cv::Mat& output,
                                             int operation, int kernelSize, 
                                             int iterations) {
    if (!isValidImage(input)) {
        output = input.clone();
        return;
    }
    
    // Ensure odd kernel size
    if (kernelSize % 2 == 0) kernelSize++;
    kernelSize = std::max(3, std::min(21, kernelSize));
    
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, 
                                               cv::Size(kernelSize, kernelSize));
    
    cv::Mat temp = input.clone();
    for (int i = 0; i < iterations; ++i) {
        cv::morphologyEx(temp, temp, operation, kernel);
    }
    output = temp;
}

// =============================================================================
// EDGE DETECTION SUITE
// =============================================================================

void MorphologyLib::applyPrewittOperator(const cv::Mat& input, cv::Mat& output) {
    if (!isValidImage(input)) {
        output = input.clone();
        return;
    }
    
    bool wasColor = (input.channels() == 3);
    
    // Convert to grayscale if needed
    cv::Mat gray;
    if (wasColor) {
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = input.clone();
    }
    
    // Prewitt kernels
    cv::Mat prewittX = (cv::Mat_<float>(3, 3) << 
        -1, 0, 1,
        -1, 0, 1,
        -1, 0, 1);
    
    cv::Mat prewittY = (cv::Mat_<float>(3, 3) << 
        -1, -1, -1,
         0,  0,  0,
         1,  1,  1);
    
    // Apply kernels
    cv::Mat gradX, gradY;
    cv::filter2D(gray, gradX, CV_32F, prewittX);
    cv::filter2D(gray, gradY, CV_32F, prewittY);
    
    // Compute magnitude
    cv::Mat magnitude;
    cv::magnitude(gradX, gradY, magnitude);
    
    // Normalize to 0-255
    cv::Mat grayOutput;
    normalizeEdgeImage(magnitude, grayOutput);
    
    // Convert back to color if input was color
    if (wasColor) {
        cv::cvtColor(grayOutput, output, cv::COLOR_GRAY2BGR);
    } else {
        output = grayOutput;
    }
}

void MorphologyLib::applyRobertsCross(const cv::Mat& input, cv::Mat& output) {
    if (!isValidImage(input)) {
        output = input.clone();
        return;
    }
    
    bool wasColor = (input.channels() == 3);
    
    // Convert to grayscale if needed
    cv::Mat gray;
    if (wasColor) {
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = input.clone();
    }
    
    // Roberts Cross kernels (2x2)
    cv::Mat robertsX = (cv::Mat_<float>(2, 2) << 
        1,  0,
        0, -1);
    
    cv::Mat robertsY = (cv::Mat_<float>(2, 2) << 
        0,  1,
       -1,  0);
    
    // Apply kernels
    cv::Mat gradX, gradY;
    cv::filter2D(gray, gradX, CV_32F, robertsX);
    cv::filter2D(gray, gradY, CV_32F, robertsY);
    
    // Compute magnitude
    cv::Mat magnitude;
    cv::magnitude(gradX, gradY, magnitude);
    
    // Normalize to 0-255
    cv::Mat grayOutput;
    normalizeEdgeImage(magnitude, grayOutput);
    
    // Convert back to color if input was color
    if (wasColor) {
        cv::cvtColor(grayOutput, output, cv::COLOR_GRAY2BGR);
    } else {
        output = grayOutput;
    }
}

void MorphologyLib::applyLoG(const cv::Mat& input, cv::Mat& output, 
                             int kernelSize, double sigma) {
    if (!isValidImage(input)) {
        output = input.clone();
        return;
    }
    
    bool wasColor = (input.channels() == 3);
    
    // Convert to grayscale if needed
    cv::Mat gray;
    if (wasColor) {
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = input.clone();
    }
    
    // Ensure odd kernel size
    if (kernelSize % 2 == 0) kernelSize++;
    kernelSize = std::max(3, std::min(31, kernelSize));
    
    // Apply Gaussian blur first
    cv::Mat blurred;
    cv::GaussianBlur(gray, blurred, cv::Size(kernelSize, kernelSize), sigma);
    
    // Apply Laplacian
    cv::Mat laplacian;
    cv::Laplacian(blurred, laplacian, CV_32F);
    
    // Normalize to 0-255
    cv::Mat grayOutput;
    normalizeEdgeImage(laplacian, grayOutput);
    
    // Convert back to color if input was color
    if (wasColor) {
        cv::cvtColor(grayOutput, output, cv::COLOR_GRAY2BGR);
    } else {
        output = grayOutput;
    }
}

void MorphologyLib::applyDoG(const cv::Mat& input, cv::Mat& output,
                             int kernelSize1, double sigma1,
                             int kernelSize2, double sigma2) {
    if (!isValidImage(input)) {
        output = input.clone();
        return;
    }
    
    bool wasColor = (input.channels() == 3);
    
    // Convert to grayscale if needed
    cv::Mat gray;
    if (wasColor) {
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = input.clone();
    }
    
    // Ensure odd kernel sizes
    if (kernelSize1 % 2 == 0) kernelSize1++;
    if (kernelSize2 % 2 == 0) kernelSize2++;
    kernelSize1 = std::max(3, std::min(31, kernelSize1));
    kernelSize2 = std::max(3, std::min(31, kernelSize2));
    
    // Apply two Gaussian blurs
    cv::Mat blur1, blur2;
    cv::GaussianBlur(gray, blur1, cv::Size(kernelSize1, kernelSize1), sigma1);
    cv::GaussianBlur(gray, blur2, cv::Size(kernelSize2, kernelSize2), sigma2);
    
    // Ensure both blurs have same size (they should, but be safe)
    if (blur1.size() != blur2.size()) {
        output = input.clone();
        return;
    }
    
    // Convert to same type before subtraction
    blur1.convertTo(blur1, CV_32F);
    blur2.convertTo(blur2, CV_32F);
    
    // Compute difference
    cv::Mat diff;
    cv::subtract(blur1, blur2, diff);
    
    // Normalize to 0-255
    cv::Mat grayOutput;
    normalizeEdgeImage(diff, grayOutput);
    
    // Convert back to color if input was color
    if (wasColor) {
        cv::cvtColor(grayOutput, output, cv::COLOR_GRAY2BGR);
    } else {
        output = grayOutput;
    }
}

void MorphologyLib::applyZeroCrossing(const cv::Mat& input, cv::Mat& output, 
                                      int kernelSize) {
    if (!isValidImage(input)) {
        output = input.clone();
        return;
    }
    
    bool wasColor = (input.channels() == 3);
    
    // Convert to grayscale if needed
    cv::Mat gray;
    if (wasColor) {
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = input.clone();
    }
    
    // Ensure odd kernel size
    if (kernelSize % 2 == 0) kernelSize++;
    kernelSize = std::max(3, std::min(31, kernelSize));
    
    // Convert to float and apply Laplacian
    cv::Mat grayFloat;
    gray.convertTo(grayFloat, CV_32F);
    cv::Mat laplacian;
    cv::Laplacian(grayFloat, laplacian, CV_32F, kernelSize);
    
    // Detect zero crossings
    cv::Mat grayOutput = cv::Mat::zeros(laplacian.size(), CV_8U);
    
    for (int i = 1; i < laplacian.rows - 1; ++i) {
        for (int j = 1; j < laplacian.cols - 1; ++j) {
            float center = laplacian.at<float>(i, j);
            
            // Check for zero crossing with neighbors
            bool zeroCrossing = false;
            
            // Check horizontal
            if ((center > 0 && laplacian.at<float>(i, j+1) < 0) ||
                (center < 0 && laplacian.at<float>(i, j+1) > 0)) {
                zeroCrossing = true;
            }
            
            // Check vertical
            if ((center > 0 && laplacian.at<float>(i+1, j) < 0) ||
                (center < 0 && laplacian.at<float>(i+1, j) > 0)) {
                zeroCrossing = true;
            }
            
            // Check diagonals
            if ((center > 0 && laplacian.at<float>(i+1, j+1) < 0) ||
                (center < 0 && laplacian.at<float>(i+1, j+1) > 0)) {
                zeroCrossing = true;
            }
            
            if ((center > 0 && laplacian.at<float>(i+1, j-1) < 0) ||
                (center < 0 && laplacian.at<float>(i+1, j-1) > 0)) {
                zeroCrossing = true;
            }
            
            if (zeroCrossing) {
                grayOutput.at<uchar>(i, j) = 255;
            }
        }
    }
    
    // Convert back to color if input was color
    if (wasColor) {
        cv::cvtColor(grayOutput, output, cv::COLOR_GRAY2BGR);
    } else {
        output = grayOutput;
    }
}

// =============================================================================
// UTILITY FUNCTIONS
// =============================================================================

bool MorphologyLib::isValidImage(const cv::Mat& image) {
    return !image.empty() && (image.channels() == 1 || image.channels() == 3);
}

void MorphologyLib::normalizeEdgeImage(const cv::Mat& input, cv::Mat& output) {
    // Convert to absolute values if needed
    cv::Mat absImage;
    if (input.depth() == CV_32F || input.depth() == CV_64F) {
        absImage = cv::abs(input);
    } else {
        absImage = input;
    }
    
    // Normalize to 0-255
    cv::normalize(absImage, output, 0, 255, cv::NORM_MINMAX, CV_8U);
}
