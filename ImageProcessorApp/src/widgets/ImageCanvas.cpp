#include "ImageCanvas.h"
#include <QPainter>
#include <QResizeEvent>

ImageCanvas::ImageCanvas(QWidget *parent, const QString& borderColor)
    : QWidget(parent), borderColor(borderColor) {
    
    setMinimumSize(400, 300);
    setStyleSheet(QString("background-color: #0f1535; "
                         "border: 2px solid %1; "
                         "border-radius: 4px;").arg(borderColor));
    
    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("border: none; background: transparent;");
    imageLabel->setText("No Image Loaded");
    imageLabel->setWordWrap(true);
    
    // Center the label initially
    imageLabel->move(10, 10);
    imageLabel->resize(size() - QSize(20, 20));
}

void ImageCanvas::setImage(const QPixmap& pixmap) {
    currentPixmap = pixmap;
    updateScaledPixmap();
}

void ImageCanvas::setImage(const cv::Mat& mat) {
    if (mat.empty()) {
        clear();
        return;
    }
    
    // Convert cv::Mat to QPixmap
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
    
    QImage qImg(rgb.data, rgb.cols, rgb.rows, rgb.step, 
                QImage::Format_RGB888);
    currentPixmap = QPixmap::fromImage(qImg.copy());
    updateScaledPixmap();
}

void ImageCanvas::clear() {
    currentPixmap = QPixmap();
    imageLabel->clear();
    imageLabel->setText("No Image Loaded");
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("color: #7a8399; font-size: 12pt; border: none; background: transparent;");
    update();
}

void ImageCanvas::updateScaledPixmap() {
    if (currentPixmap.isNull()) return;
    
    QSize canvasSize = size() - QSize(20, 20); // Padding
    scaledPixmap = currentPixmap.scaled(canvasSize, 
                                       Qt::KeepAspectRatio, 
                                       Qt::SmoothTransformation);
    
    imageLabel->setPixmap(scaledPixmap);
    imageLabel->adjustSize();
    
    // Center the label
    int x = (width() - imageLabel->width()) / 2;
    int y = (height() - imageLabel->height()) / 2;
    imageLabel->move(x, y);
}

void ImageCanvas::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    if (!currentPixmap.isNull()) {
        updateScaledPixmap();
    } else {
        // Update text label position
        imageLabel->resize(size() - QSize(20, 20));
        imageLabel->move(10, 10);
    }
}

void ImageCanvas::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
}