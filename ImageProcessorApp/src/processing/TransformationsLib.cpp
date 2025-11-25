#include "TransformationsLib.h"

namespace TransformationsLib {

void applyTranslation(const cv::Mat& input, cv::Mat& output, int tx, int ty) {
    cv::Mat M = (cv::Mat_<float>(2, 3) << 1, 0, tx, 0, 1, ty);
    cv::warpAffine(input, output, M, input.size());
}

void applyRotation(const cv::Mat& input, cv::Mat& output, double angle) {
    cv::Point2f center(input.cols / 2.0f, input.rows / 2.0f);
    cv::Mat M = cv::getRotationMatrix2D(center, angle, 1.0);
    cv::warpAffine(input, output, M, input.size());
}

void applyZoom(const cv::Mat& input, cv::Mat& output, double zoomFactor) {
    cv::Mat resized;
    cv::resize(input, resized, cv::Size(), zoomFactor, zoomFactor);
    
    // If zoomed out, pad with black
    if (zoomFactor < 1.0) {
        output = cv::Mat::zeros(input.size(), input.type());
        int x_offset = (input.cols - resized.cols) / 2;
        int y_offset = (input.rows - resized.rows) / 2;
        
        cv::Rect roi(x_offset, y_offset, resized.cols, resized.rows);
        resized.copyTo(output(roi));
    }
    // If zoomed in, crop to original size
    else if (zoomFactor > 1.0) {
        int x_offset = (resized.cols - input.cols) / 2;
        int y_offset = (resized.rows - input.rows) / 2;
        
        cv::Rect roi(x_offset, y_offset, input.cols, input.rows);
        output = resized(roi).clone();
    } else {
        output = input.clone();
    }
}

void applyFlipX(const cv::Mat& input, cv::Mat& output) {
    cv::flip(input, output, 0); // Flip around x-axis
}

void applyFlipY(const cv::Mat& input, cv::Mat& output) {
    cv::flip(input, output, 1); // Flip around y-axis
}

void applyFlipXY(const cv::Mat& input, cv::Mat& output) {
    cv::flip(input, output, -1); // Flip both axes
}

void applySkew(const cv::Mat& input, cv::Mat& output, float shearX) {
    int rows = input.rows;
    int cols = input.cols;
    
    cv::Point2f srcTri[3];
    cv::Point2f dstTri[3];
    
    srcTri[0] = cv::Point2f(0, 0);
    srcTri[1] = cv::Point2f(cols - 1, 0);
    srcTri[2] = cv::Point2f(0, rows - 1);
    
    dstTri[0] = cv::Point2f(0, 0);
    dstTri[1] = cv::Point2f(cols - 1, 0);
    dstTri[2] = cv::Point2f(shearX, rows - 1);
    
    cv::Mat warpMat = cv::getAffineTransform(srcTri, dstTri);
    cv::warpAffine(input, output, warpMat, input.size());
}

} // namespace TransformationsLib
