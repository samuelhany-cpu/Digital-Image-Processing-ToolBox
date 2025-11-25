#ifndef IMAGECANVAS_H
#define IMAGECANVAS_H

#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QPainter>
#include <QResizeEvent>
#include <opencv2/opencv.hpp>

class ImageCanvas : public QWidget {
    Q_OBJECT

public:
    explicit ImageCanvas(QWidget *parent = nullptr, 
                        const QString& borderColor = "#00d4ff");
    
    void setImage(const QPixmap& pixmap);
    void setImage(const cv::Mat& mat);
    void clear();
    QPixmap getPixmap() const { return currentPixmap; }
    
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    
private:
    void updateScaledPixmap();
    
    QLabel *imageLabel;
    QPixmap currentPixmap;
    QPixmap scaledPixmap;
    QString borderColor;
};

#endif // IMAGECANVAS_H