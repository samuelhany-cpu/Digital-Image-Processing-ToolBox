#ifndef TRANSFORMATIONSLIB_H
#define TRANSFORMATIONSLIB_H

#include <opencv2/opencv.hpp>

namespace TransformationsLib {

/**
 * @brief Apply translation transformation
 * @param input Input image
 * @param output Output transformed image
 * @param tx Translation in X direction (pixels)
 * @param ty Translation in Y direction (pixels)
 */
void applyTranslation(const cv::Mat& input, cv::Mat& output, int tx, int ty);

/**
 * @brief Apply rotation transformation
 * @param input Input image
 * @param output Output transformed image
 * @param angle Rotation angle in degrees
 */
void applyRotation(const cv::Mat& input, cv::Mat& output, double angle);

/**
 * @brief Apply zoom/scaling transformation
 * @param input Input image
 * @param output Output transformed image
 * @param zoomFactor Zoom factor (1.0 = no change, >1.0 = zoom in, <1.0 = zoom out)
 */
void applyZoom(const cv::Mat& input, cv::Mat& output, double zoomFactor);

/**
 * @brief Apply horizontal flip
 * @param input Input image
 * @param output Output flipped image
 */
void applyFlipX(const cv::Mat& input, cv::Mat& output);

/**
 * @brief Apply vertical flip
 * @param input Input image
 * @param output Output flipped image
 */
void applyFlipY(const cv::Mat& input, cv::Mat& output);

/**
 * @brief Apply both horizontal and vertical flip
 * @param input Input image
 * @param output Output flipped image
 */
void applyFlipXY(const cv::Mat& input, cv::Mat& output);

/**
 * @brief Apply skew/shear transformation
 * @param input Input image
 * @param output Output transformed image
 * @param shearX Horizontal shear amount (default: 100)
 */
void applySkew(const cv::Mat& input, cv::Mat& output, float shearX = 100.0f);

} // namespace TransformationsLib

#endif // TRANSFORMATIONSLIB_H
