#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <opencv2/opencv.hpp>
#include <QString>
#include <QPixmap>

namespace ImageUtils {

/**
 * @brief Convert OpenCV Mat to QPixmap for Qt display
 * @param mat OpenCV Mat image
 * @return QPixmap for Qt widgets
 */
QPixmap cvMatToQPixmap(const cv::Mat& mat);

/**
 * @brief Convert QPixmap to OpenCV Mat
 * @param pixmap Qt QPixmap image
 * @return OpenCV Mat
 */
cv::Mat qPixmapToCvMat(const QPixmap& pixmap);

/**
 * @brief Get image information as formatted string
 * @param image OpenCV Mat image
 * @param filePath Path to the image file
 * @return Formatted information string
 */
QString getImageInfo(const cv::Mat& image, const QString& filePath);

/**
 * @brief Calculate image statistics
 * @param image OpenCV Mat image
 * @param mean Output mean value
 * @param stdDev Output standard deviation
 * @param minVal Output minimum value
 * @param maxVal Output maximum value
 */
void calculateImageStatistics(const cv::Mat& image, 
                              double& mean, double& stdDev,
                              double& minVal, double& maxVal);

/**
 * @brief Get pixel value at coordinates
 * @param image OpenCV Mat image
 * @param x X coordinate
 * @param y Y coordinate
 * @return Pixel value as QString
 */
QString getPixelValue(const cv::Mat& image, int x, int y);

} // namespace ImageUtils

#endif // IMAGEUTILS_H
