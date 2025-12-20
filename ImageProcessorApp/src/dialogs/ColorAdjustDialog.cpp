#include "ColorAdjustDialog.h"
#include "../processing/ColorProcessingLib.h"
#include <QGridLayout>
#include <QMessageBox>

ColorAdjustDialog::ColorAdjustDialog(const cv::Mat& originalImage, QWidget *parent)
    : QDialog(parent),
      originalImage(originalImage.clone()),
      brightnessValue(0),
      contrastValue(1.0),
      saturationValue(100),
      hueValue(0),
      temperatureValue(0),
      livePreviewEnabled(true)
{
    setWindowTitle("Color Adjustments");
    setModal(true);
    resize(500, 600);
    
    setupUI();
    applyStyleSheet();
    
    // Initialize adjusted image
    adjustedImage = originalImage.clone();
}

ColorAdjustDialog::~ColorAdjustDialog() {
}

cv::Mat ColorAdjustDialog::getAdjustedImage() const {
    return adjustedImage;
}

void ColorAdjustDialog::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // Title label
    QLabel *titleLabel = new QLabel("Adjust Color Properties");
    titleLabel->setStyleSheet("font-size: 16pt; font-weight: bold; color: #00ffff;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // Brightness control
    QGroupBox *brightnessGroup = createSliderGroup("Brightness", -100, 100, 0, 
                                                   brightnessSlider, brightnessLabel);
    mainLayout->addWidget(brightnessGroup);
    connect(brightnessSlider, &QSlider::valueChanged, this, &ColorAdjustDialog::onBrightnessChanged);
    
    // Contrast control (slider range 50-300 for 0.5-3.0)
    contrastSlider = new QSlider(Qt::Horizontal);
    contrastSlider->setRange(50, 300);
    contrastSlider->setValue(100);
    contrastSlider->setTickPosition(QSlider::TicksBelow);
    contrastSlider->setTickInterval(25);
    
    contrastLabel = new QLabel("1.00");
    contrastLabel->setAlignment(Qt::AlignCenter);
    contrastLabel->setMinimumWidth(60);
    
    QGroupBox *contrastGroup = new QGroupBox("Contrast");
    QHBoxLayout *contrastLayout = new QHBoxLayout();
    contrastLayout->addWidget(contrastSlider);
    contrastLayout->addWidget(contrastLabel);
    contrastGroup->setLayout(contrastLayout);
    mainLayout->addWidget(contrastGroup);
    connect(contrastSlider, &QSlider::valueChanged, this, &ColorAdjustDialog::onContrastChanged);
    
    // Saturation control
    QGroupBox *saturationGroup = createSliderGroup("Saturation (%)", 0, 200, 100, 
                                                   saturationSlider, saturationLabel);
    mainLayout->addWidget(saturationGroup);
    connect(saturationSlider, &QSlider::valueChanged, this, &ColorAdjustDialog::onSaturationChanged);
    
    // Hue control
    QGroupBox *hueGroup = createSliderGroup("Hue Shift (degrees)", 0, 360, 0, 
                                           hueSlider, hueLabel);
    mainLayout->addWidget(hueGroup);
    connect(hueSlider, &QSlider::valueChanged, this, &ColorAdjustDialog::onHueChanged);
    
    // Temperature control
    QGroupBox *temperatureGroup = createSliderGroup("Temperature", -100, 100, 0, 
                                                    temperatureSlider, temperatureLabel);
    mainLayout->addWidget(temperatureGroup);
    connect(temperatureSlider, &QSlider::valueChanged, this, &ColorAdjustDialog::onTemperatureChanged);
    
    // Live preview checkbox
    livePreviewCheckBox = new QCheckBox("Live Preview");
    livePreviewCheckBox->setChecked(true);
    mainLayout->addWidget(livePreviewCheckBox);
    connect(livePreviewCheckBox, &QCheckBox::toggled, [this](bool checked) {
        livePreviewEnabled = checked;
    });
    
    // Add spacing
    mainLayout->addStretch();
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);
    
    resetButton = new QPushButton("Reset");
    resetButton->setMinimumHeight(35);
    connect(resetButton, &QPushButton::clicked, this, &ColorAdjustDialog::onResetClicked);
    
    cancelButton = new QPushButton("Cancel");
    cancelButton->setMinimumHeight(35);
    connect(cancelButton, &QPushButton::clicked, this, &ColorAdjustDialog::onCancelClicked);
    
    applyButton = new QPushButton("Apply");
    applyButton->setMinimumHeight(35);
    applyButton->setDefault(true);
    connect(applyButton, &QPushButton::clicked, this, &ColorAdjustDialog::onApplyClicked);
    
    buttonLayout->addWidget(resetButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(applyButton);
    
    mainLayout->addLayout(buttonLayout);
}

QGroupBox* ColorAdjustDialog::createSliderGroup(const QString& label, int min, int max, 
                                                int defaultValue, QSlider*& slider, 
                                                QLabel*& valueLabel) {
    QGroupBox *group = new QGroupBox(label);
    
    slider = new QSlider(Qt::Horizontal);
    slider->setRange(min, max);
    slider->setValue(defaultValue);
    slider->setTickPosition(QSlider::TicksBelow);
    slider->setTickInterval((max - min) / 10);
    
    valueLabel = new QLabel(QString::number(defaultValue));
    valueLabel->setAlignment(Qt::AlignCenter);
    valueLabel->setMinimumWidth(60);
    
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(slider);
    layout->addWidget(valueLabel);
    group->setLayout(layout);
    
    return group;
}

void ColorAdjustDialog::applyStyleSheet() {
    setStyleSheet(R"(
        QDialog {
            background-color: #0a0e27;
            color: #ffffff;
        }
        
        QGroupBox {
            border: 2px solid #1a2351;
            border-radius: 8px;
            margin-top: 12px;
            padding-top: 15px;
            font-weight: bold;
            color: #00ffff;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px;
        }
        
        QSlider::groove:horizontal {
            border: 1px solid #1a2351;
            height: 8px;
            background: #0f1535;
            border-radius: 4px;
        }
        
        QSlider::handle:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #00ffff, stop:1 #00cccc);
            border: 1px solid #00aaaa;
            width: 18px;
            margin: -5px 0;
            border-radius: 9px;
        }
        
        QSlider::handle:horizontal:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #00ffff, stop:1 #00ffff);
        }
        
        QSlider::add-page:horizontal {
            background: #0f1535;
        }
        
        QSlider::sub-page:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #00ffff, stop:1 #0088ff);
        }
        
        QLabel {
            color: #ffffff;
            font-size: 11pt;
        }
        
        QCheckBox {
            color: #ffffff;
            font-size: 11pt;
            spacing: 8px;
        }
        
        QCheckBox::indicator {
            width: 20px;
            height: 20px;
            border: 2px solid #1a2351;
            border-radius: 4px;
            background: #0f1535;
        }
        
        QCheckBox::indicator:checked {
            background: #00ffff;
            border-color: #00ffff;
        }
        
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1a2351, stop:1 #0f1535);
            color: #ffffff;
            border: 2px solid #00ffff;
            border-radius: 8px;
            padding: 8px 20px;
            font-size: 11pt;
            font-weight: bold;
            min-width: 100px;
        }
        
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #2a3361, stop:1 #1a2351);
            border-color: #00ffff;
        }
        
        QPushButton:pressed {
            background: #0f1535;
        }
        
        QPushButton:default {
            border-color: #00ff00;
        }
    )");
}

void ColorAdjustDialog::onBrightnessChanged(int value) {
    brightnessValue = value;
    brightnessLabel->setText(QString::number(value));
    
    if (livePreviewEnabled) {
        updatePreview();
    }
}

void ColorAdjustDialog::onContrastChanged(int value) {
    contrastValue = value / 100.0;
    contrastLabel->setText(QString::number(contrastValue, 'f', 2));
    
    if (livePreviewEnabled) {
        updatePreview();
    }
}

void ColorAdjustDialog::onSaturationChanged(int value) {
    saturationValue = value;
    saturationLabel->setText(QString::number(value));
    
    if (livePreviewEnabled) {
        updatePreview();
    }
}

void ColorAdjustDialog::onHueChanged(int value) {
    hueValue = value;
    hueLabel->setText(QString::number(value));
    
    if (livePreviewEnabled) {
        updatePreview();
    }
}

void ColorAdjustDialog::onTemperatureChanged(int value) {
    temperatureValue = value;
    temperatureLabel->setText(QString::number(value));
    
    if (livePreviewEnabled) {
        updatePreview();
    }
}

void ColorAdjustDialog::onResetClicked() {
    // Reset all sliders to default values
    brightnessSlider->setValue(0);
    contrastSlider->setValue(100);
    saturationSlider->setValue(100);
    hueSlider->setValue(0);
    temperatureSlider->setValue(0);
    
    // Reset values
    brightnessValue = 0;
    contrastValue = 1.0;
    saturationValue = 100;
    hueValue = 0;
    temperatureValue = 0;
    
    // Reset image
    adjustedImage = originalImage.clone();
    
    updatePreview();
}

void ColorAdjustDialog::onApplyClicked() {
    // Process final image
    adjustedImage = processImage();
    accept();
}

void ColorAdjustDialog::onCancelClicked() {
    // Restore original image
    adjustedImage = originalImage.clone();
    reject();
}

void ColorAdjustDialog::updatePreview() {
    adjustedImage = processImage();
    emit previewRequested(adjustedImage);
}

cv::Mat ColorAdjustDialog::processImage() {
    if (originalImage.empty()) {
        return cv::Mat();
    }
    
    cv::Mat result;
    
    try {
        // Apply all adjustments
        // For color adjustments (saturation, hue), we need 3-channel image
        if (originalImage.channels() == 3) {
            // Apply brightness and contrast first
            cv::Mat temp1, temp2;
            ColorProcessingLib::adjustBrightness(originalImage, temp1, brightnessValue);
            ColorProcessingLib::adjustContrast(temp1, temp2, contrastValue);
            
            // Apply saturation
            cv::Mat temp3;
            ColorProcessingLib::adjustSaturation(temp2, temp3, saturationValue);
            
            // Apply hue shift
            cv::Mat temp4;
            ColorProcessingLib::adjustHue(temp3, temp4, hueValue);
            
            // Apply temperature
            ColorProcessingLib::adjustTemperature(temp4, result, temperatureValue);
        } else {
            // For grayscale images, only brightness and contrast apply
            cv::Mat temp;
            ColorProcessingLib::adjustBrightness(originalImage, temp, brightnessValue);
            ColorProcessingLib::adjustContrast(temp, result, contrastValue);
        }
        
        return result;
    } catch (const cv::Exception& e) {
        QMessageBox::warning(const_cast<ColorAdjustDialog*>(this), 
                           "Processing Error", 
                           QString("Error adjusting colors: %1").arg(e.what()));
        return originalImage.clone();
    }
}
