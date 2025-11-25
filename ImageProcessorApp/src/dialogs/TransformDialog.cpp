#include "TransformDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>

TransformDialog::TransformDialog(QWidget *parent, 
                                TransformType type,
                                const cv::Mat& inputImage)
    : QDialog(parent), transformType(type), sourceImage(inputImage.clone()) {
    
    setWindowTitle("Image Transformation");
    setModal(true);
    setMinimumSize(700, 450);
    
    // Apply dark theme styling
    setStyleSheet("QDialog { background-color: #0a0e27; }");
    
    switch (transformType) {
        case Translation:
            setupTranslationUI();
            break;
        case Rotation:
            setupRotationUI();
            break;
        case Zoom:
            setupZoomUI();
            break;
    }
}

void TransformDialog::setupTranslationUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel *titleLabel = new QLabel("Image Translation Settings");
    titleLabel->setStyleSheet("font-size: 14pt; font-weight: bold; "
                             "color: #00d4ff; padding: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // X Translation Section
    QGroupBox *xGroup = new QGroupBox("X Translation (Horizontal)");
    QVBoxLayout *xLayout = new QVBoxLayout(xGroup);
    
    int maxOffset = std::max(sourceImage.cols, sourceImage.rows) / 2;
    
    sliderX = new QSlider(Qt::Horizontal);
    sliderX->setRange(-maxOffset, maxOffset);
    sliderX->setValue(0);
    xLayout->addWidget(sliderX);
    
    QHBoxLayout *xInputLayout = new QHBoxLayout();
    xInputLayout->addWidget(new QLabel("Value (pixels):"));
    spinBoxX = new QSpinBox();
    spinBoxX->setRange(-maxOffset, maxOffset);
    spinBoxX->setValue(0);
    xInputLayout->addWidget(spinBoxX);
    xInputLayout->addWidget(new QLabel("Right  |  Left"));
    xInputLayout->addStretch();
    xLayout->addLayout(xInputLayout);
    
    mainLayout->addWidget(xGroup);
    
    // Y Translation Section
    QGroupBox *yGroup = new QGroupBox("Y Translation (Vertical)");
    QVBoxLayout *yLayout = new QVBoxLayout(yGroup);
    
    sliderY = new QSlider(Qt::Horizontal);
    sliderY->setRange(-maxOffset, maxOffset);
    sliderY->setValue(0);
    sliderY->setStyleSheet("QSlider::handle:horizontal { "
                          "background: #1fa65a; }");
    yLayout->addWidget(sliderY);
    
    QHBoxLayout *yInputLayout = new QHBoxLayout();
    yInputLayout->addWidget(new QLabel("Value (pixels):"));
    spinBoxY = new QSpinBox();
    spinBoxY->setRange(-maxOffset, maxOffset);
    spinBoxY->setValue(0);
    yInputLayout->addWidget(spinBoxY);
    yInputLayout->addWidget(new QLabel("Down  |  Up"));
    yInputLayout->addStretch();
    yLayout->addLayout(yInputLayout);
    
    mainLayout->addWidget(yGroup);
    
    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    
    QPushButton *cancelBtn = new QPushButton("Cancel");
    QPushButton *applyBtn = new QPushButton("Apply Translation");
    applyBtn->setProperty("class", "accent");
    
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(applyBtn);
    mainLayout->addLayout(btnLayout);
    
    // Connect signals
    connect(sliderX, &QSlider::valueChanged, 
            spinBoxX, &QSpinBox::setValue);
    connect(spinBoxX, QOverload<int>::of(&QSpinBox::valueChanged),
            sliderX, &QSlider::setValue);
    connect(sliderX, &QSlider::valueChanged, 
            this, &TransformDialog::onParameterChanged);
    
    connect(sliderY, &QSlider::valueChanged, 
            spinBoxY, &QSpinBox::setValue);
    connect(spinBoxY, QOverload<int>::of(&QSpinBox::valueChanged),
            sliderY, &QSlider::setValue);
    connect(sliderY, &QSlider::valueChanged, 
            this, &TransformDialog::onParameterChanged);
    
    connect(applyBtn, &QPushButton::clicked, 
            this, &TransformDialog::applyTransform);
    connect(cancelBtn, &QPushButton::clicked, 
            this, &QDialog::reject);
    
    // Trigger initial preview
    onParameterChanged();
}

void TransformDialog::setupRotationUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel *titleLabel = new QLabel("Image Rotation Settings");
    titleLabel->setStyleSheet("font-size: 14pt; font-weight: bold; "
                             "color: #00d4ff; padding: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Rotation Section
    QGroupBox *rotGroup = new QGroupBox("Rotation Angle");
    QVBoxLayout *rotLayout = new QVBoxLayout(rotGroup);
    
    angleSlider = new QSlider(Qt::Horizontal);
    angleSlider->setRange(-180, 180);
    angleSlider->setValue(0);
    angleSlider->setStyleSheet("QSlider::handle:horizontal { "
                              "background: #ff006e; }");
    rotLayout->addWidget(angleSlider);
    
    QHBoxLayout *angleInputLayout = new QHBoxLayout();
    angleInputLayout->addWidget(new QLabel("Angle (degrees):"));
    angleSpinBox = new QDoubleSpinBox();
    angleSpinBox->setRange(-180.0, 180.0);
    angleSpinBox->setValue(0.0);
    angleSpinBox->setSuffix(" deg");
    angleInputLayout->addWidget(angleSpinBox);
    angleInputLayout->addWidget(new QLabel("Clockwise  |  Counter-clockwise"));
    angleInputLayout->addStretch();
    rotLayout->addLayout(angleInputLayout);
    
    mainLayout->addWidget(rotGroup);
    
    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    
    QPushButton *cancelBtn = new QPushButton("Cancel");
    QPushButton *applyBtn = new QPushButton("Apply Rotation");
    applyBtn->setProperty("class", "accent");
    
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(applyBtn);
    mainLayout->addLayout(btnLayout);
    
    // Connect signals
    connect(angleSlider, &QSlider::valueChanged, 
            [this](int value) { angleSpinBox->setValue(value); });
    connect(angleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [this](double value) { angleSlider->setValue((int)value); });
    connect(angleSlider, &QSlider::valueChanged, 
            this, &TransformDialog::onParameterChanged);
    
    connect(applyBtn, &QPushButton::clicked, 
            this, &TransformDialog::applyTransform);
    connect(cancelBtn, &QPushButton::clicked, 
            this, &QDialog::reject);
    
    // Trigger initial preview
    onParameterChanged();
}

void TransformDialog::setupZoomUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel *titleLabel = new QLabel("Image Zoom Settings");
    titleLabel->setStyleSheet("font-size: 14pt; font-weight: bold; "
                             "color: #00d4ff; padding: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Zoom Section
    QGroupBox *zoomGroup = new QGroupBox("Zoom Factor");
    QVBoxLayout *zoomLayout = new QVBoxLayout(zoomGroup);
    
    zoomSlider = new QSlider(Qt::Horizontal);
    zoomSlider->setRange(25, 300); // 0.25x to 3.0x
    zoomSlider->setValue(100); // 1.0x
    zoomSlider->setStyleSheet("QSlider::handle:horizontal { "
                             "background: #39ff14; }");
    zoomLayout->addWidget(zoomSlider);
    
    QHBoxLayout *zoomInputLayout = new QHBoxLayout();
    zoomInputLayout->addWidget(new QLabel("Zoom factor:"));
    zoomSpinBox = new QDoubleSpinBox();
    zoomSpinBox->setRange(0.25, 3.0);
    zoomSpinBox->setValue(1.0);
    zoomSpinBox->setSuffix("x");
    zoomSpinBox->setSingleStep(0.1);
    zoomInputLayout->addWidget(zoomSpinBox);
    zoomInputLayout->addWidget(new QLabel("Zoom In  |  Zoom Out"));
    zoomInputLayout->addStretch();
    zoomLayout->addLayout(zoomInputLayout);
    
    mainLayout->addWidget(zoomGroup);
    
    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    
    QPushButton *cancelBtn = new QPushButton("Cancel");
    QPushButton *applyBtn = new QPushButton("Apply Zoom");
    applyBtn->setProperty("class", "accent");
    
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(applyBtn);
    mainLayout->addLayout(btnLayout);
    
    // Connect signals
    connect(zoomSlider, &QSlider::valueChanged, 
            [this](int value) { zoomSpinBox->setValue(value / 100.0); });
    connect(zoomSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [this](double value) { zoomSlider->setValue((int)(value * 100)); });
    connect(zoomSlider, &QSlider::valueChanged, 
            this, &TransformDialog::onParameterChanged);
    
    connect(applyBtn, &QPushButton::clicked, 
            this, &TransformDialog::applyTransform);
    connect(cancelBtn, &QPushButton::clicked, 
            this, &QDialog::reject);
    
    // Trigger initial preview
    onParameterChanged();
}

void TransformDialog::onParameterChanged() {
    switch (transformType) {
        case Translation:
            applyTranslationPreview();
            break;
        case Rotation:
            applyRotationPreview();
            break;
        case Zoom:
            applyZoomPreview();
            break;
    }
}

void TransformDialog::applyTranslationPreview() {
    int tx = spinBoxX->value();
    int ty = spinBoxY->value();
    
    cv::Mat M = (cv::Mat_<float>(2, 3) << 1, 0, tx, 0, 1, ty);
    cv::warpAffine(sourceImage, resultImage, M, 
                   sourceImage.size());
    
    emit previewRequested(resultImage);
}

void TransformDialog::applyRotationPreview() {
    double angle = angleSpinBox->value();
    
    cv::Point2f center(sourceImage.cols / 2.0, sourceImage.rows / 2.0);
    cv::Mat M = cv::getRotationMatrix2D(center, angle, 1.0);
    cv::warpAffine(sourceImage, resultImage, M, sourceImage.size());
    
    emit previewRequested(resultImage);
}

void TransformDialog::applyZoomPreview() {
    double zoom = zoomSpinBox->value();
    
    cv::resize(sourceImage, resultImage, cv::Size(), zoom, zoom);
    
    // If zoomed out, pad with black
    if (zoom < 1.0) {
        cv::Mat padded = cv::Mat::zeros(sourceImage.size(), sourceImage.type());
        int x_offset = (sourceImage.cols - resultImage.cols) / 2;
        int y_offset = (sourceImage.rows - resultImage.rows) / 2;
        
        cv::Rect roi(x_offset, y_offset, resultImage.cols, resultImage.rows);
        resultImage.copyTo(padded(roi));
        resultImage = padded;
    }
    // If zoomed in, crop to original size
    else if (zoom > 1.0) {
        int x_offset = (resultImage.cols - sourceImage.cols) / 2;
        int y_offset = (resultImage.rows - sourceImage.rows) / 2;
        
        cv::Rect roi(x_offset, y_offset, sourceImage.cols, sourceImage.rows);
        resultImage = resultImage(roi);
    }
    
    emit previewRequested(resultImage);
}

void TransformDialog::applyTransform() {
    // Result already computed in preview
    accept();
}