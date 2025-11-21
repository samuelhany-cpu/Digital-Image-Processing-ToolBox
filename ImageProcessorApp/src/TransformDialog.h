#ifndef TRANSFORMDIALOG_H
#define TRANSFORMDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <opencv2/opencv.hpp>
#include <functional>

class TransformDialog : public QDialog {
    Q_OBJECT

public:
    enum TransformType {
        Translation,
        Rotation,
        Zoom
    };
    
    explicit TransformDialog(QWidget *parent, 
                            TransformType type,
                            const cv::Mat& inputImage);
    
    cv::Mat getResultImage() const { return resultImage; }
    
signals:
    void previewRequested(const cv::Mat& preview);

private slots:
    void onParameterChanged();
    void applyTransform();

private:
    void setupTranslationUI();
    void setupRotationUI();
    void setupZoomUI();
    
    void applyTranslationPreview();
    void applyRotationPreview();
    void applyZoomPreview();
    
    TransformType transformType;
    cv::Mat sourceImage;
    cv::Mat resultImage;
    
    // UI elements
    QSlider *sliderX;
    QSlider *sliderY;
    QSlider *angleSlider;
    QSlider *scaleSlider;
    QSlider *zoomSlider;
    
    QSpinBox *spinBoxX;
    QSpinBox *spinBoxY;
    QDoubleSpinBox *angleSpinBox;
    QDoubleSpinBox *scaleSpinBox;
    QDoubleSpinBox *zoomSpinBox;
};

#endif // TRANSFORMDIALOG_H