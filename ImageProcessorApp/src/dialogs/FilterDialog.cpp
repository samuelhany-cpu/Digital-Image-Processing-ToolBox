#include "FilterDialog.h"
#include "../filters/ImageFilters.h"
#include <QGridLayout>
#include <QMessageBox>

FilterDialog::FilterDialog(const cv::Mat& originalImage, FilterType filterType, QWidget *parent)
    : QDialog(parent),
      originalImage(originalImage.clone()),
      filterType(filterType),
      medianKernelSize(5),
      bilateralD(9),
      bilateralSigmaColor(75.0),
      bilateralSigmaSpace(75.0),
      nlmH(10.0f),
      nlmTemplateWindow(7),
      nlmSearchWindow(21),
      morphKernelSize(5),
      morphKernelShape(1),
      unsharpSigma(1.0),
      unsharpAmount(1.5),
      unsharpThreshold(0),
      highPassKernelSize(21),
      sharpenStrength(100),
      livePreviewEnabled(true),
      slider1(nullptr),
      slider2(nullptr),
      slider3(nullptr),
      label1(nullptr),
      label2(nullptr),
      label3(nullptr),
      shapeCombo(nullptr)
{
    setWindowTitle("Advanced Filter Settings");
    setModal(true);
    resize(500, 400);
    
    setupUI();
    applyStyleSheet();
    
    filteredImage = originalImage.clone();
}

FilterDialog::~FilterDialog() {
}

cv::Mat FilterDialog::getFilteredImage() const {
    return filteredImage;
}

void FilterDialog::setupUI() {
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // Title based on filter type
    QString titleText;
    switch (filterType) {
        case MEDIAN: titleText = "Median Filter Settings"; break;
        case BILATERAL: titleText = "Bilateral Filter Settings"; break;
        case NON_LOCAL_MEANS: titleText = "Non-Local Means Settings"; break;
        case MORPHOLOGICAL_OPENING: titleText = "Morphological Opening"; break;
        case MORPHOLOGICAL_CLOSING: titleText = "Morphological Closing"; break;
        case MORPHOLOGICAL_GRADIENT: titleText = "Morphological Gradient"; break;
        case TOP_HAT: titleText = "Top-Hat Transform"; break;
        case BLACK_HAT: titleText = "Black-Hat Transform"; break;
        case UNSHARP_MASK: titleText = "Unsharp Mask Settings"; break;
        case HIGH_PASS: titleText = "High-Pass Filter"; break;
        case CUSTOM_SHARPEN: titleText = "Custom Sharpen"; break;
    }
    
    QLabel *titleLabel = new QLabel(titleText);
    titleLabel->setStyleSheet("font-size: 16pt; font-weight: bold; color: #00ffff;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // Setup parameter controls based on filter type
    switch (filterType) {
        case MEDIAN:
            setupMedianUI();
            break;
        case BILATERAL:
            setupBilateralUI();
            break;
        case NON_LOCAL_MEANS:
            setupNLMUI();
            break;
        case MORPHOLOGICAL_OPENING:
        case MORPHOLOGICAL_CLOSING:
            setupMorphologicalUI();
            break;
        case MORPHOLOGICAL_GRADIENT:
        case TOP_HAT:
        case BLACK_HAT:
            setupMorphologicalUI();
            break;
        case UNSHARP_MASK:
            setupUnsharpUI();
            break;
        case HIGH_PASS:
            setupHighPassUI();
            break;
        case CUSTOM_SHARPEN:
            setupSharpenUI();
            break;
    }
    
    // Live preview checkbox
    livePreviewCheckBox = new QCheckBox("Live Preview");
    livePreviewCheckBox->setChecked(true);
    mainLayout->addWidget(livePreviewCheckBox);
    connect(livePreviewCheckBox, &QCheckBox::toggled, [this](bool checked) {
        livePreviewEnabled = checked;
    });
    
    mainLayout->addStretch();
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);
    
    resetButton = new QPushButton("Reset");
    resetButton->setMinimumHeight(35);
    connect(resetButton, &QPushButton::clicked, this, &FilterDialog::onResetClicked);
    
    cancelButton = new QPushButton("Cancel");
    cancelButton->setMinimumHeight(35);
    connect(cancelButton, &QPushButton::clicked, this, &FilterDialog::onCancelClicked);
    
    applyButton = new QPushButton("Apply");
    applyButton->setMinimumHeight(35);
    applyButton->setDefault(true);
    connect(applyButton, &QPushButton::clicked, this, &FilterDialog::onApplyClicked);
    
    buttonLayout->addWidget(resetButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(applyButton);
    
    mainLayout->addLayout(buttonLayout);
}

void FilterDialog::setupMedianUI() {
    parametersGroup = new QGroupBox("Kernel Size");
    QVBoxLayout *layout = new QVBoxLayout(parametersGroup);
    
    QGroupBox *kernelGroup = createSliderGroup("Kernel Size (odd)", 3, 9, 5, slider1, label1);
    layout->addWidget(kernelGroup);
    
    connect(slider1, &QSlider::valueChanged, [this](int value) {
        // Ensure odd value
        if (value % 2 == 0) value++;
        medianKernelSize = value;
        label1->setText(QString::number(value));
        onParameterChanged();
    });
    
    mainLayout->addWidget(parametersGroup);
}

void FilterDialog::setupBilateralUI() {
    parametersGroup = new QGroupBox("Filter Parameters");
    QVBoxLayout *layout = new QVBoxLayout(parametersGroup);
    
    QGroupBox *dGroup = createSliderGroup("Diameter (d)", 3, 15, 9, slider1, label1);
    QGroupBox *sigmaColorGroup = createSliderGroup("Sigma Color", 10, 200, 75, slider2, label2);
    QGroupBox *sigmaSpaceGroup = createSliderGroup("Sigma Space", 10, 200, 75, slider3, label3);
    
    layout->addWidget(dGroup);
    layout->addWidget(sigmaColorGroup);
    layout->addWidget(sigmaSpaceGroup);
    
    connect(slider1, &QSlider::valueChanged, [this](int value) {
        bilateralD = value;
        label1->setText(QString::number(value));
        onParameterChanged();
    });
    
    connect(slider2, &QSlider::valueChanged, [this](int value) {
        bilateralSigmaColor = value;
        label2->setText(QString::number(value));
        onParameterChanged();
    });
    
    connect(slider3, &QSlider::valueChanged, [this](int value) {
        bilateralSigmaSpace = value;
        label3->setText(QString::number(value));
        onParameterChanged();
    });
    
    mainLayout->addWidget(parametersGroup);
}

void FilterDialog::setupNLMUI() {
    parametersGroup = new QGroupBox("Denoising Parameters");
    QVBoxLayout *layout = new QVBoxLayout(parametersGroup);
    
    QGroupBox *hGroup = createSliderGroup("Filter Strength (h)", 1, 30, 10, slider1, label1);
    QGroupBox *templateGroup = createSliderGroup("Template Window", 5, 11, 7, slider2, label2);
    QGroupBox *searchGroup = createSliderGroup("Search Window", 11, 31, 21, slider3, label3);
    
    layout->addWidget(hGroup);
    layout->addWidget(templateGroup);
    layout->addWidget(searchGroup);
    
    connect(slider1, &QSlider::valueChanged, [this](int value) {
        nlmH = static_cast<float>(value);
        label1->setText(QString::number(value));
        onParameterChanged();
    });
    
    connect(slider2, &QSlider::valueChanged, [this](int value) {
        // Ensure odd value
        if (value % 2 == 0) value++;
        nlmTemplateWindow = value;
        label2->setText(QString::number(value));
        onParameterChanged();
    });
    
    connect(slider3, &QSlider::valueChanged, [this](int value) {
        // Ensure odd value
        if (value % 2 == 0) value++;
        nlmSearchWindow = value;
        label3->setText(QString::number(value));
        onParameterChanged();
    });
    
    mainLayout->addWidget(parametersGroup);
}

void FilterDialog::setupMorphologicalUI() {
    parametersGroup = new QGroupBox("Morphological Parameters");
    QVBoxLayout *layout = new QVBoxLayout(parametersGroup);
    
    QGroupBox *kernelGroup = createSliderGroup("Kernel Size", 3, 21, 5, slider1, label1);
    layout->addWidget(kernelGroup);
    
    // Shape selection (only for opening/closing)
    if (filterType == MORPHOLOGICAL_OPENING || filterType == MORPHOLOGICAL_CLOSING) {
        QLabel *shapeLabel = new QLabel("Kernel Shape:");
        shapeCombo = new QComboBox();
        shapeCombo->addItem("Rectangle", 0);
        shapeCombo->addItem("Ellipse", 1);
        shapeCombo->addItem("Cross", 2);
        shapeCombo->setCurrentIndex(1); // Default to Ellipse
        
        QHBoxLayout *shapeLayout = new QHBoxLayout();
        shapeLayout->addWidget(shapeLabel);
        shapeLayout->addWidget(shapeCombo);
        layout->addLayout(shapeLayout);
        
        connect(shapeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
                [this](int index) {
            morphKernelShape = shapeCombo->currentData().toInt();
            onParameterChanged();
        });
    }
    
    connect(slider1, &QSlider::valueChanged, [this](int value) {
        // Ensure odd value
        if (value % 2 == 0) value++;
        morphKernelSize = value;
        label1->setText(QString::number(value));
        onParameterChanged();
    });
    
    mainLayout->addWidget(parametersGroup);
}

void FilterDialog::setupUnsharpUI() {
    parametersGroup = new QGroupBox("Unsharp Mask Parameters");
    QVBoxLayout *layout = new QVBoxLayout(parametersGroup);
    
    // Sigma (0.1 to 5.0)
    QGroupBox *sigmaGroup = new QGroupBox("Gaussian Sigma");
    QHBoxLayout *sigmaLayout = new QHBoxLayout();
    slider1 = new QSlider(Qt::Horizontal);
    slider1->setRange(1, 50); // 0.1 to 5.0
    slider1->setValue(10); // 1.0
    label1 = new QLabel("1.0");
    sigmaLayout->addWidget(slider1);
    sigmaLayout->addWidget(label1);
    sigmaGroup->setLayout(sigmaLayout);
    layout->addWidget(sigmaGroup);
    
    // Amount (0.1 to 3.0)
    QGroupBox *amountGroup = new QGroupBox("Sharpening Amount");
    QHBoxLayout *amountLayout = new QHBoxLayout();
    slider2 = new QSlider(Qt::Horizontal);
    slider2->setRange(1, 30); // 0.1 to 3.0
    slider2->setValue(15); // 1.5
    label2 = new QLabel("1.5");
    amountLayout->addWidget(slider2);
    amountLayout->addWidget(label2);
    amountGroup->setLayout(amountLayout);
    layout->addWidget(amountGroup);
    
    // Threshold (0 to 50)
    QGroupBox *thresholdGroup = createSliderGroup("Threshold", 0, 50, 0, slider3, label3);
    layout->addWidget(thresholdGroup);
    
    connect(slider1, &QSlider::valueChanged, [this](int value) {
        unsharpSigma = value / 10.0;
        label1->setText(QString::number(unsharpSigma, 'f', 1));
        onParameterChanged();
    });
    
    connect(slider2, &QSlider::valueChanged, [this](int value) {
        unsharpAmount = value / 10.0;
        label2->setText(QString::number(unsharpAmount, 'f', 1));
        onParameterChanged();
    });
    
    connect(slider3, &QSlider::valueChanged, [this](int value) {
        unsharpThreshold = value;
        label3->setText(QString::number(value));
        onParameterChanged();
    });
    
    mainLayout->addWidget(parametersGroup);
}

void FilterDialog::setupHighPassUI() {
    parametersGroup = new QGroupBox("High-Pass Parameters");
    QVBoxLayout *layout = new QVBoxLayout(parametersGroup);
    
    QGroupBox *kernelGroup = createSliderGroup("Kernel Size (odd)", 3, 51, 21, slider1, label1);
    layout->addWidget(kernelGroup);
    
    connect(slider1, &QSlider::valueChanged, [this](int value) {
        // Ensure odd value
        if (value % 2 == 0) value++;
        highPassKernelSize = value;
        label1->setText(QString::number(value));
        onParameterChanged();
    });
    
    mainLayout->addWidget(parametersGroup);
}

void FilterDialog::setupSharpenUI() {
    parametersGroup = new QGroupBox("Sharpening Strength");
    QVBoxLayout *layout = new QVBoxLayout(parametersGroup);
    
    QGroupBox *strengthGroup = createSliderGroup("Strength (%)", 0, 200, 100, slider1, label1);
    layout->addWidget(strengthGroup);
    
    connect(slider1, &QSlider::valueChanged, [this](int value) {
        sharpenStrength = value;
        label1->setText(QString::number(value));
        onParameterChanged();
    });
    
    mainLayout->addWidget(parametersGroup);
}

QGroupBox* FilterDialog::createSliderGroup(const QString& label, int min, int max, 
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

void FilterDialog::applyStyleSheet() {
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
        
        QComboBox {
            background: #0f1535;
            color: #ffffff;
            border: 2px solid #1a2351;
            border-radius: 4px;
            padding: 5px;
            min-width: 120px;
        }
        
        QComboBox:hover {
            border-color: #00ffff;
        }
        
        QComboBox::drop-down {
            border: none;
        }
        
        QComboBox QAbstractItemView {
            background: #0f1535;
            color: #ffffff;
            selection-background-color: #1a2351;
            border: 2px solid #00ffff;
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

void FilterDialog::onParameterChanged() {
    if (livePreviewEnabled) {
        updatePreview();
    }
}

void FilterDialog::onResetClicked() {
    // Reset to default values based on filter type
    switch (filterType) {
        case MEDIAN:
            medianKernelSize = 5;
            if (slider1) slider1->setValue(5);
            break;
        case BILATERAL:
            bilateralD = 9;
            bilateralSigmaColor = 75.0;
            bilateralSigmaSpace = 75.0;
            if (slider1) slider1->setValue(9);
            if (slider2) slider2->setValue(75);
            if (slider3) slider3->setValue(75);
            break;
        case NON_LOCAL_MEANS:
            nlmH = 10.0f;
            nlmTemplateWindow = 7;
            nlmSearchWindow = 21;
            if (slider1) slider1->setValue(10);
            if (slider2) slider2->setValue(7);
            if (slider3) slider3->setValue(21);
            break;
        case MORPHOLOGICAL_OPENING:
        case MORPHOLOGICAL_CLOSING:
        case MORPHOLOGICAL_GRADIENT:
        case TOP_HAT:
        case BLACK_HAT:
            morphKernelSize = 5;
            morphKernelShape = 1;
            if (slider1) slider1->setValue(5);
            if (shapeCombo) shapeCombo->setCurrentIndex(1);
            break;
        case UNSHARP_MASK:
            unsharpSigma = 1.0;
            unsharpAmount = 1.5;
            unsharpThreshold = 0;
            if (slider1) slider1->setValue(10);
            if (slider2) slider2->setValue(15);
            if (slider3) slider3->setValue(0);
            break;
        case HIGH_PASS:
            highPassKernelSize = 21;
            if (slider1) slider1->setValue(21);
            break;
        case CUSTOM_SHARPEN:
            sharpenStrength = 100;
            if (slider1) slider1->setValue(100);
            break;
    }
    
    filteredImage = originalImage.clone();
    updatePreview();
}

void FilterDialog::onApplyClicked() {
    filteredImage = processImage();
    accept();
}

void FilterDialog::onCancelClicked() {
    filteredImage = originalImage.clone();
    reject();
}

void FilterDialog::updatePreview() {
    filteredImage = processImage();
    emit previewRequested(filteredImage);
}

cv::Mat FilterDialog::processImage() {
    if (originalImage.empty()) {
        return cv::Mat();
    }
    
    cv::Mat result;
    
    try {
        switch (filterType) {
            case MEDIAN:
                ImageFilters::applyMedianFilter(originalImage, result, medianKernelSize);
                break;
            case BILATERAL:
                ImageFilters::applyBilateralFilter(originalImage, result, 
                                                   bilateralD, bilateralSigmaColor, bilateralSigmaSpace);
                break;
            case NON_LOCAL_MEANS:
                ImageFilters::applyNonLocalMeansDenoising(originalImage, result, 
                                                          nlmH, nlmTemplateWindow, nlmSearchWindow);
                break;
            case MORPHOLOGICAL_OPENING:
                ImageFilters::applyMorphologicalOpening(originalImage, result, 
                                                       morphKernelSize, morphKernelShape);
                break;
            case MORPHOLOGICAL_CLOSING:
                ImageFilters::applyMorphologicalClosing(originalImage, result, 
                                                       morphKernelSize, morphKernelShape);
                break;
            case MORPHOLOGICAL_GRADIENT:
                ImageFilters::applyMorphologicalGradient(originalImage, result, morphKernelSize);
                break;
            case TOP_HAT:
                ImageFilters::applyTopHat(originalImage, result, morphKernelSize);
                break;
            case BLACK_HAT:
                ImageFilters::applyBlackHat(originalImage, result, morphKernelSize);
                break;
            case UNSHARP_MASK:
                ImageFilters::applyUnsharpMask(originalImage, result, 
                                              unsharpSigma, unsharpAmount, unsharpThreshold);
                break;
            case HIGH_PASS:
                ImageFilters::applyHighPassFilter(originalImage, result, highPassKernelSize);
                break;
            case CUSTOM_SHARPEN:
                ImageFilters::applyCustomSharpen(originalImage, result, sharpenStrength);
                break;
        }
        
        return result;
    } catch (const cv::Exception& e) {
        QMessageBox::warning(const_cast<FilterDialog*>(this), 
                           "Processing Error", 
                           QString("Error applying filter: %1").arg(e.what()));
        return originalImage.clone();
    }
}
