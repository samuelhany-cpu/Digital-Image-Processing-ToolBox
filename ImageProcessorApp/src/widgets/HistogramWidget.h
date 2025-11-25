#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <opencv2/opencv.hpp>
#include <vector>

class HistogramWidget : public QWidget {
    Q_OBJECT

public:
    explicit HistogramWidget(QWidget *parent = nullptr);
    
    void setImage(const cv::Mat& image);
    void clear();
    
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void calculateHistogram();
    void drawHistogram(QPainter& painter);
    
    cv::Mat sourceImage;
    std::vector<int> histogramData[3]; // RGB channels
    int maxFrequency;
    bool isGrayscale;
};

#endif // HISTOGRAMWIDGET_H