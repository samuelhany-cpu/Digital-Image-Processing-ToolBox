#include "ImageProcessingLib.h"

namespace ImageProcessingLib {

// Auto Enhancement function - Advanced version
void applyAutoEnhance(const cv::Mat& input, cv::Mat& output, QStringList& operations) {
    operations.clear();
    
    cv::Mat result = input.clone();
    cv::Mat gray;
    
    // Convert to grayscale for analysis
    if (result.channels() == 3) {
        cv::cvtColor(result, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = result.clone();
    }
    
    // === STEP 1: Analyze image characteristics ===
    double minVal, maxVal, meanVal;
    cv::minMaxLoc(gray, &minVal, &maxVal);
    cv::Scalar meanScalar = cv::mean(gray);
    meanVal = meanScalar[0];
    
    cv::Scalar mean, stddev;
    cv::meanStdDev(gray, mean, stddev);
    double contrast = stddev[0];
    
    // Calculate dynamic range
    double dynamicRange = maxVal - minVal;
    
    // === STEP 2: Brightness & Contrast Correction ===
    bool needsBrightnessAdjustment = false;
    bool needsContrastAdjustment = false;
    
    // Check if image is too dark (mean < 100) or too bright (mean > 180)
    if (meanVal < 100.0) {
        double alpha = 1.2; // Slight contrast increase
        int beta = static_cast<int>((120.0 - meanVal) * 0.8); // Adaptive brightness
        result.convertTo(result, -1, alpha, beta);
        operations << QString("Brightness +%1, Contrast x%2").arg(beta).arg(alpha);
        needsBrightnessAdjustment = true;
    } else if (meanVal > 180.0) {
        double alpha = 1.1;
        int beta = static_cast<int>((120.0 - meanVal) * 0.5); // Reduce brightness
        result.convertTo(result, -1, alpha, beta);
        operations << QString("Brightness %1, Contrast x%2").arg(beta).arg(alpha);
        needsBrightnessAdjustment = true;
    }
    
    // === STEP 3: Contrast Enhancement ===
    // Low contrast detection (stddev < 50)
    if (contrast < 50.0 && !needsBrightnessAdjustment) {
        // Apply histogram equalization for low contrast images
        applyHistogramEqualization(result, result);
        operations << "Histogram Equalization (Low Contrast)";
        needsContrastAdjustment = true;
    } else if (dynamicRange < 150 && !needsBrightnessAdjustment) {
        // Stretch dynamic range if too narrow
        double alpha = 255.0 / dynamicRange;
        int beta = static_cast<int>(-minVal * alpha);
        result.convertTo(result, -1, alpha, beta);
        operations << "Dynamic Range Stretch";
        needsContrastAdjustment = true;
    }
    
    // === STEP 4: Noise Reduction ===
    // Detect noise level using Laplacian variance
    cv::Mat laplacian;
    cv::Laplacian(gray, laplacian, CV_64F);
    cv::Scalar lmean, lstddev;
    cv::meanStdDev(laplacian, lmean, lstddev);
    double edgeVariance = lstddev[0] * lstddev[0];
    
    // If low edge variance but high frequency noise
    if (edgeVariance > 200.0) {
        // Apply bilateral filter (preserves edges while removing noise)
        cv::Mat denoised;
        cv::bilateralFilter(result, denoised, 9, 75, 75);
        result = denoised;
        operations << "Bilateral Noise Reduction";
    } else if (edgeVariance > 100.0) {
        // Light Gaussian blur for moderate noise
        cv::GaussianBlur(result, result, cv::Size(3, 3), 0);
        operations << "Gaussian Noise Reduction (3x3)";
    }
    
    // === STEP 5: Sharpening ===
    // Check if image needs sharpening (low edge variance)
    if (edgeVariance < 80.0) {
        // Unsharp masking for soft images
        cv::Mat blurred, sharpened;
        cv::GaussianBlur(result, blurred, cv::Size(0, 0), 2.0);
        cv::addWeighted(result, 1.8, blurred, -0.8, 0, sharpened);
        
        // Clip values to valid range
        sharpened.convertTo(result, result.type());
        cv::max(result, cv::Scalar(0), result);
        cv::min(result, cv::Scalar(255), result);
        
        operations << "Unsharp Masking (2.0 sigma)";
    }
    
    // === STEP 6: Color Saturation Boost (for color images) ===
    if (result.channels() == 3) {
        cv::Mat hsv;
        cv::cvtColor(result, hsv, cv::COLOR_BGR2HSV);
        
        std::vector<cv::Mat> channels;
        cv::split(hsv, channels);
        
        // Check average saturation
        cv::Scalar satMean = cv::mean(channels[1]);
        
        if (satMean[0] < 100.0) {
            // Boost saturation for dull images
            channels[1] = channels[1] * 1.3;
            cv::merge(channels, hsv);
            cv::cvtColor(hsv, result, cv::COLOR_HSV2BGR);
            operations << "Saturation Boost x1.3";
        }
    }
    
    // === STEP 7: Final Contrast Adjustment ===
    // Apply CLAHE (Contrast Limited Adaptive Histogram Equalization) if needed
    if (result.channels() == 3) {
        cv::cvtColor(result, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = result.clone();
    }
    cv::meanStdDev(gray, mean, stddev);
    
    if (stddev[0] < 45.0 && !needsContrastAdjustment) {
        // Apply CLAHE for localized contrast enhancement
        if (result.channels() == 3) {
            cv::Mat lab;
            cv::cvtColor(result, lab, cv::COLOR_BGR2Lab);
            
            std::vector<cv::Mat> labChannels;
            cv::split(lab, labChannels);
            
            cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(8, 8));
            clahe->apply(labChannels[0], labChannels[0]);
            
            cv::merge(labChannels, lab);
            cv::cvtColor(lab, result, cv::COLOR_Lab2BGR);
            operations << "CLAHE (Adaptive Contrast)";
        } else {
            cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(8, 8));
            clahe->apply(result, result);
            operations << "CLAHE (Adaptive Contrast)";
        }
    }
    
    output = result.clone();
    
    // Add summary
    if (operations.isEmpty()) {
        operations << "Image Already Well-Balanced (No Changes Needed)";
    }
}

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
