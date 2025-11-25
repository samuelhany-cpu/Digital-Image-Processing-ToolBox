#include "ImageUtils.h"
#include <QImage>

namespace ImageUtils {

QPixmap cvMatToQPixmap(const cv::Mat& mat) {
    if (mat.empty()) return QPixmap();
    
    cv::Mat rgb;
    if (mat.channels() == 1) {
        cv::cvtColor(mat, rgb, cv::COLOR_GRAY2RGB);
    } else if (mat.channels() == 3) {
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
    } else if (mat.channels() == 4) {
        cv::cvtColor(mat, rgb, cv::COLOR_BGRA2RGBA);
    } else {
        rgb = mat.clone();
    }
    
    QImage qImg(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
    return QPixmap::fromImage(qImg.copy());
}

cv::Mat qPixmapToCvMat(const QPixmap& pixmap) {
    QImage qImg = pixmap.toImage().convertToFormat(QImage::Format_RGB888);
    cv::Mat mat(qImg.height(), qImg.width(), CV_8UC3, 
                (void*)qImg.constBits(), qImg.bytesPerLine());
    cv::Mat result;
    cv::cvtColor(mat, result, cv::COLOR_RGB2BGR);
    return result.clone();
}

QString getImageInfo(const cv::Mat& image, const QString& filePath) {
    int rows = image.rows;
    int cols = image.cols;
    int channels = image.channels();
    
    QString imgType;
    if (channels == 1) {
        imgType = "Grayscale";
    } else if (channels == 3) {
        imgType = "Color (RGB/BGR)";
    } else if (channels == 4) {
        imgType = "Color with Alpha (RGBA/BGRA)";
    } else {
        imgType = QString("Multi-channel (%1)").arg(channels);
    }
    
    double minVal, maxVal, meanVal;
    cv::minMaxLoc(image, &minVal, &maxVal);
    meanVal = cv::mean(image)[0];
    
    QString info;
    info += "=======================================\n\n";
    info += QString("  File Path:               %1\n\n").arg(filePath);
    info += QString("  Dimensions (WxH):        %1 x %2\n\n")
           .arg(cols, 6).arg(rows, -6);
    info += QString("  Total Pixels:            %1\n\n")
           .arg(QString::number(rows * cols), 15);
    info += QString("  Image Type:              %1\n")
           .arg(imgType, 20);
    info += QString("  Channels:                %1\n\n")
           .arg(channels, 20);
    info += QString("  Data Type:               %1\n\n")
           .arg(QString::fromStdString(
               cv::typeToString(image.type())), 20);
    info += QString("  Min Value:               %1\n")
           .arg(minVal, 20, 'f', 2);
    info += QString("  Max Value:               %1\n")
           .arg(maxVal, 20, 'f', 2);
    info += QString("  Mean Value:              %1\n\n")
           .arg(meanVal, 20, 'f', 2);
    info += "=======================================";
    
    return info;
}

void calculateImageStatistics(const cv::Mat& image, 
                              double& mean, double& stdDev,
                              double& minVal, double& maxVal) {
    cv::minMaxLoc(image, &minVal, &maxVal);
    
    cv::Scalar meanScalar = cv::mean(image);
    mean = meanScalar[0];
    
    cv::Mat meanMat, stdDevMat;
    cv::meanStdDev(image, meanMat, stdDevMat);
    stdDev = stdDevMat.at<double>(0, 0);
}

QString getPixelValue(const cv::Mat& image, int x, int y) {
    QString valueStr;
    
    if (x < 0 || x >= image.cols || y < 0 || y >= image.rows) {
        return "Out of bounds";
    }
    
    if (image.channels() == 1) {
        uchar val = image.at<uchar>(y, x);
        valueStr = QString::number(val);
    } else if (image.channels() == 3) {
        cv::Vec3b val = image.at<cv::Vec3b>(y, x);
        valueStr = QString("B:%1, G:%2, R:%3")
                  .arg(val[0]).arg(val[1]).arg(val[2]);
    } else if (image.channels() == 4) {
        cv::Vec4b val = image.at<cv::Vec4b>(y, x);
        valueStr = QString("B:%1, G:%2, R:%3, A:%4")
                  .arg(val[0]).arg(val[1]).arg(val[2]).arg(val[3]);
    }
    
    return valueStr;
}

} // namespace ImageUtils
