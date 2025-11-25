#include "HistogramWidget.h"
#include <QPainter>
#include <QPainterPath>

HistogramWidget::HistogramWidget(QWidget *parent)
    : QWidget(parent), maxFrequency(0), isGrayscale(true) {
    
    setMinimumSize(600, 400);
    setStyleSheet("background-color: #0f1535; "
                 "border: 1px solid #3a4a6f; "
                 "border-radius: 8px;");
}

void HistogramWidget::setImage(const cv::Mat& image) {
    sourceImage = image.clone();
    calculateHistogram();
    update();
}

void HistogramWidget::calculateHistogram() {
    if (sourceImage.empty()) return;
    
    // Clear previous data
    for (int i = 0; i < 3; i++) {
        histogramData[i].clear();
        histogramData[i].resize(256, 0);
    }
    
    maxFrequency = 0;
    isGrayscale = (sourceImage.channels() == 1);
    
    if (isGrayscale) {
        // Grayscale histogram
        for (int i = 0; i < sourceImage.rows; i++) {
            for (int j = 0; j < sourceImage.cols; j++) {
                uchar pixel = sourceImage.at<uchar>(i, j);
                histogramData[0][pixel]++;
                if (histogramData[0][pixel] > maxFrequency) {
                    maxFrequency = histogramData[0][pixel];
                }
            }
        }
    } else {
        // Color histogram (BGR)
        for (int i = 0; i < sourceImage.rows; i++) {
            for (int j = 0; j < sourceImage.cols; j++) {
                cv::Vec3b pixel = sourceImage.at<cv::Vec3b>(i, j);
                for (int c = 0; c < 3; c++) {
                    histogramData[c][pixel[c]]++;
                    if (histogramData[c][pixel[c]] > maxFrequency) {
                        maxFrequency = histogramData[c][pixel[c]];
                    }
                }
            }
        }
    }
}

void HistogramWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    
    if (sourceImage.empty() || maxFrequency == 0) {
        // Draw empty state
        QPainter painter(this);
        painter.setPen(QColor("#7a8399"));
        painter.setFont(QFont("Segoe UI", 12));
        painter.drawText(rect(), Qt::AlignCenter, "No Image Data");
        return;
    }
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    drawHistogram(painter);
}

void HistogramWidget::drawHistogram(QPainter& painter) {
    int margin = 40;
    int width = this->width() - 2 * margin;
    int height = this->height() - 2 * margin;
    
    // Draw axes
    painter.setPen(QPen(QColor("#3a4a6f"), 1));
    painter.drawLine(margin, this->height() - margin, 
                    this->width() - margin, this->height() - margin); // X-axis
    painter.drawLine(margin, margin, margin, this->height() - margin); // Y-axis
    
    // Draw grid
    painter.setPen(QPen(QColor("#1f2845"), 1, Qt::DashLine));
    for (int i = 1; i < 5; i++) {
        int y = margin + (height * i / 5);
        painter.drawLine(margin, y, this->width() - margin, y);
    }
    
    // Draw histogram bars
    float barWidth = (float)width / 256.0f;
    
    if (isGrayscale) {
        // Cyan gradient for grayscale
        QLinearGradient gradient(0, margin, 0, this->height() - margin);
        gradient.setColorAt(0, QColor("#00d4ff"));
        gradient.setColorAt(1, QColor("#0099cc"));
        
        painter.setPen(Qt::NoPen);
        painter.setBrush(gradient);
        
        for (int i = 0; i < 256; i++) {
            float barHeight = (float)histogramData[0][i] / maxFrequency * height;
            float x = margin + i * barWidth;
            float y = this->height() - margin - barHeight;
            
            painter.drawRect(QRectF(x, y, barWidth, barHeight));
        }
    } else {
        // RGB channels with different colors
        QColor channelColors[3] = {
            QColor("#00d4ff"), // Blue - Cyan
            QColor("#39ff14"), // Green - Lime
            QColor("#ff006e")  // Red - Pink
        };
        
        for (int c = 0; c < 3; c++) {
            painter.setPen(QPen(channelColors[c], 1));
            
            QPainterPath path;
            path.moveTo(margin, this->height() - margin);
            
            for (int i = 0; i < 256; i++) {
                float barHeight = (float)histogramData[c][i] / maxFrequency * height;
                float x = margin + i * barWidth;
                float y = this->height() - margin - barHeight;
                
                if (i == 0) {
                    path.lineTo(x, y);
                } else {
                    path.lineTo(x, y);
                }
            }
            
            path.lineTo(this->width() - margin, this->height() - margin);
            
            painter.setOpacity(0.3);
            painter.fillPath(path, channelColors[c]);
            painter.setOpacity(1.0);
            painter.drawPath(path);
        }
    }
    
    // Draw labels
    painter.setPen(QColor("#9ca3b3"));
    painter.setFont(QFont("Segoe UI", 9));
    painter.drawText(margin - 10, this->height() - margin + 20, "0");
    painter.drawText(this->width() - margin - 20, this->height() - margin + 20, "255");
    painter.drawText(10, margin + 5, QString::number(maxFrequency));
    
    // Title
    painter.setPen(QColor("#00d4ff"));
    painter.setFont(QFont("Segoe UI", 11, QFont::Bold));
    painter.drawText(margin, 20, "Pixel Value Distribution");
}

void HistogramWidget::clear() {
    sourceImage = cv::Mat();
    maxFrequency = 0;
    update();
}