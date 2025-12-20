#include "MainWindow.h"
#include "ImageCanvas.h"
#include "TransformDialog.h"
#include "HistogramWidget.h"
#include "../dialogs/ColorAdjustDialog.h"
#include "../processing/ColorProcessingLib.h"
#include <QApplication>
#include <QSplitter>
#include <QScrollArea>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), imageLoaded(false), recentlyProcessed(false) {
    
    setWindowTitle("SamToolbox - Professional Image Processing");
    setWindowIcon(QIcon(":/icons/mexo_toolbox_logo.ico"));
    setMinimumSize(1200, 800);
    resize(1600, 1000);
    
    setupUI();
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    createMenuBar();
    createToolBar();
    createCentralWidget();
    createStatusBar();
}

void MainWindow::createMenuBar() {
    QMenuBar *menuBar = this->menuBar();
    
    // File Menu
    QMenu *fileMenu = menuBar->addMenu("File");
    
    loadAction = new QAction("Load Image", this);
    loadAction->setShortcut(QKeySequence::Open);
    loadAction->setToolTip("Load an image file");
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadImage);
    
    saveAction = new QAction("Save Image", this);
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setToolTip("Save processed image");
    saveAction->setEnabled(false);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveImage);
    
    resetAction = new QAction("Reset", this);
    resetAction->setShortcut(QKeySequence::Refresh);
    resetAction->setToolTip("Reset to original image");
    resetAction->setEnabled(false);
    connect(resetAction, &QAction::triggered, this, &MainWindow::resetImage);
    
    exitAction = new QAction("Exit", this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    
    fileMenu->addAction(loadAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addAction(resetAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    
    // Labs Menu
    QMenu *labsMenu = menuBar->addMenu("Labs");
    
    QAction *lab1Action = labsMenu->addAction("Lab 1: Image Info");
    connect(lab1Action, &QAction::triggered, this, &MainWindow::showImageInfo);
    
    QAction *lab2Action = labsMenu->addAction("Lab 2: Pixel Info");
    connect(lab2Action, &QAction::triggered, this, &MainWindow::showPixelInfo);
    
    QAction *lab3Action = labsMenu->addAction("Lab 3: Statistics");
    connect(lab3Action, &QAction::triggered, this, &MainWindow::showImageStats);
    
    QAction *lab4Action = labsMenu->addAction("Lab 4: Transformations");
    QAction *lab5Action = labsMenu->addAction("Lab 5: Histogram");
    connect(lab5Action, &QAction::triggered, this, &MainWindow::showHistogram);
    
    QAction *lab6Action = labsMenu->addAction("Lab 6: Processing");
}

void MainWindow::createToolBar() {
    QToolBar *toolBar = addToolBar("Main");
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    
    toolBar->addAction(loadAction);
    toolBar->addSeparator();
    toolBar->addAction(saveAction);
    toolBar->addAction(resetAction);
}

void MainWindow::createCentralWidget() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    
    // Create splitter for resizable layout
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, this);
    
    // Left panel for images
    QWidget *imagePanel = new QWidget();
    QVBoxLayout *imageLayout = new QVBoxLayout(imagePanel);
    
    // Image canvases
    QHBoxLayout *canvasLayout = new QHBoxLayout();
    
    // Original image section
    QVBoxLayout *originalSection = new QVBoxLayout();
    QLabel *originalTitle = new QLabel("Original Image");
    originalTitle->setStyleSheet("font-size: 12pt; font-weight: bold; color: #00d4ff; padding: 5px;");
    originalTitle->setAlignment(Qt::AlignCenter);
    
    originalCanvas = new ImageCanvas(this, "#00d4ff");
    originalCanvas->setMinimumSize(500, 400);
    
    originalInfoLabel = new QLabel("No image loaded");
    originalInfoLabel->setStyleSheet("color: #9ca3b3; font-size: 9pt; padding: 5px;");
    originalInfoLabel->setAlignment(Qt::AlignCenter);
    
    originalSection->addWidget(originalTitle);
    originalSection->addWidget(originalCanvas);
    originalSection->addWidget(originalInfoLabel);
    
    // Processed image section
    QVBoxLayout *processedSection = new QVBoxLayout();
    processedTitleLabel = new QLabel("Processed Image");
    processedTitleLabel->setStyleSheet("font-size: 12pt; font-weight: bold; color: #1fa65a; padding: 5px;");
    processedTitleLabel->setAlignment(Qt::AlignCenter);
    
    processedCanvas = new ImageCanvas(this, "#1fa65a");
    processedCanvas->setMinimumSize(500, 400);
    
    processedInfoLabel = new QLabel("No processing applied");
    processedInfoLabel->setStyleSheet("color: #9ca3b3; font-size: 9pt; padding: 5px;");
    processedInfoLabel->setAlignment(Qt::AlignCenter);
    
    processedSection->addWidget(processedTitleLabel);
    processedSection->addWidget(processedCanvas);
    processedSection->addWidget(processedInfoLabel);
    
    canvasLayout->addLayout(originalSection);
    canvasLayout->addLayout(processedSection);
    imageLayout->addLayout(canvasLayout);
    
    // Add image panel to splitter
    mainSplitter->addWidget(imagePanel);
    
    // Right panel for controls - create it here instead of separate function
    QWidget *controlPanel = new QWidget();
    controlPanel->setMaximumWidth(350);
    controlPanel->setStyleSheet("QWidget { background-color: #0f1535; }");
    
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(controlPanel);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    QVBoxLayout *controlLayout = new QVBoxLayout(controlPanel);
    
    // Information Group
    infoGroup = new QGroupBox("Lab 1-3: Information & Analysis");
    QVBoxLayout *infoLayout = new QVBoxLayout(infoGroup);
    
    QPushButton *imageInfoBtn = new QPushButton("Image Information");
    QPushButton *pixelInfoBtn = new QPushButton("Pixel Information");
    QPushButton *statsBtn = new QPushButton("Image Statistics");
    
    addTooltip(imageInfoBtn, "Display detailed image information");
    addTooltip(pixelInfoBtn, "Get pixel values at specific coordinates");
    addTooltip(statsBtn, "Show statistical analysis of the image");
    
    connect(imageInfoBtn, &QPushButton::clicked, this, &MainWindow::showImageInfo);
    connect(pixelInfoBtn, &QPushButton::clicked, this, &MainWindow::showPixelInfo);
    connect(statsBtn, &QPushButton::clicked, this, &MainWindow::showImageStats);
    
    infoLayout->addWidget(imageInfoBtn);
    infoLayout->addWidget(pixelInfoBtn);
    infoLayout->addWidget(statsBtn);
    
    // Transformation Group
    transformGroup = new QGroupBox("Lab 4: Geometric Transformations");
    QGridLayout *transformLayout = new QGridLayout(transformGroup);
    
    QPushButton *translateBtn = new QPushButton("Translation");
    QPushButton *rotateBtn = new QPushButton("Rotation");
    QPushButton *skewBtn = new QPushButton("Skew");
    QPushButton *zoomBtn = new QPushButton("Zoom");
    QPushButton *flipXBtn = new QPushButton("Flip X");
    QPushButton *flipYBtn = new QPushButton("Flip Y");
    QPushButton *flipXYBtn = new QPushButton("Flip XY");
    
    addTooltip(translateBtn, "Move image position horizontally/vertically");
    addTooltip(rotateBtn, "Rotate image by specified angle");
    addTooltip(skewBtn, "Apply skew transformation");
    addTooltip(zoomBtn, "Zoom in/out with preview");
    addTooltip(flipXBtn, "Flip horizontally");
    addTooltip(flipYBtn, "Flip vertically");
    addTooltip(flipXYBtn, "Flip both directions");
    
    connect(translateBtn, &QPushButton::clicked, this, &MainWindow::applyTranslation);
    connect(rotateBtn, &QPushButton::clicked, this, &MainWindow::applyRotation);
    connect(skewBtn, &QPushButton::clicked, this, &MainWindow::applySkew);
    connect(zoomBtn, &QPushButton::clicked, this, &MainWindow::applyZoom);
    connect(flipXBtn, &QPushButton::clicked, this, &MainWindow::applyFlipX);
    connect(flipYBtn, &QPushButton::clicked, this, &MainWindow::applyFlipY);
    connect(flipXYBtn, &QPushButton::clicked, this, &MainWindow::applyFlipXY);
    
    transformLayout->addWidget(translateBtn, 0, 0);
    transformLayout->addWidget(rotateBtn, 0, 1);
    transformLayout->addWidget(skewBtn, 1, 0);
    transformLayout->addWidget(zoomBtn, 1, 1);
    transformLayout->addWidget(flipXBtn, 2, 0);
    transformLayout->addWidget(flipYBtn, 2, 1);
    transformLayout->addWidget(flipXYBtn, 3, 0, 1, 2);
    
    // Histogram Group
    histogramGroup = new QGroupBox("Lab 5: Histogram & Thresholding");
    QVBoxLayout *histogramLayout = new QVBoxLayout(histogramGroup);
    
    QPushButton *histogramBtn = new QPushButton("Show Histogram");
    QPushButton *equalizeBtn = new QPushButton("Histogram Equalization");
    QPushButton *otsuBtn = new QPushButton("Otsu Thresholding");
    
    addTooltip(histogramBtn, "Display image histogram with analysis");
    addTooltip(equalizeBtn, "Apply histogram equalization for contrast enhancement");
    addTooltip(otsuBtn, "Apply automatic Otsu thresholding");
    
    connect(histogramBtn, &QPushButton::clicked, this, &MainWindow::showHistogram);
    connect(equalizeBtn, &QPushButton::clicked, this, &MainWindow::applyHistogramEqualization);
    connect(otsuBtn, &QPushButton::clicked, this, &MainWindow::applyOtsuThresholding);
    
    histogramLayout->addWidget(histogramBtn);
    histogramLayout->addWidget(equalizeBtn);
    histogramLayout->addWidget(otsuBtn);
    
    // Processing Group
    processingGroup = new QGroupBox("Lab 6: Image Processing");
    QGridLayout *processingLayout = new QGridLayout(processingGroup);
    
    QPushButton *grayscaleBtn = new QPushButton("Grayscale");
    QPushButton *thresholdBtn = new QPushButton("Binary Threshold");
    QPushButton *blurBtn = new QPushButton("Gaussian Blur");
    QPushButton *edgeBtn = new QPushButton("Edge Detection");
    QPushButton *invertBtn = new QPushButton("Invert Colors");
    
    addTooltip(grayscaleBtn, "Convert image to grayscale");
    addTooltip(thresholdBtn, "Apply binary thresholding");
    addTooltip(blurBtn, "Apply Gaussian blur filter");
    addTooltip(edgeBtn, "Detect edges using Canny algorithm");
    addTooltip(invertBtn, "Invert all color values");
    
    connect(grayscaleBtn, &QPushButton::clicked, this, &MainWindow::convertToGrayscale);
    connect(thresholdBtn, &QPushButton::clicked, this, &MainWindow::applyBinaryThreshold);
    connect(blurBtn, &QPushButton::clicked, this, &MainWindow::applyGaussianBlur);
    connect(edgeBtn, &QPushButton::clicked, this, &MainWindow::applyEdgeDetection);
    connect(invertBtn, &QPushButton::clicked, this, &MainWindow::invertColors);
    
    processingLayout->addWidget(grayscaleBtn, 0, 0);
    processingLayout->addWidget(thresholdBtn, 0, 1);
    processingLayout->addWidget(blurBtn, 1, 0);
    processingLayout->addWidget(edgeBtn, 1, 1);
    processingLayout->addWidget(invertBtn, 2, 0, 1, 2);
    
    // Filters Group
    filtersGroup = new QGroupBox("Lab 7: Custom Filters");
    QGridLayout *filtersLayout = new QGridLayout(filtersGroup);
    
    QPushButton *traditionalBtn = new QPushButton("Traditional Filter");
    QPushButton *pyramidalBtn = new QPushButton("Pyramidal Filter");
    QPushButton *circularBtn = new QPushButton("Circular Filter");
    QPushButton *coneBtn = new QPushButton("Cone Filter");
    QPushButton *laplacianBtn = new QPushButton("Laplacian Filter");
    QPushButton *sobelBtn = new QPushButton("Sobel Filter");
    
    addTooltip(traditionalBtn, "Apply traditional averaging filter (mean filter)");
    addTooltip(pyramidalBtn, "Apply pyramidal weighted filter");
    addTooltip(circularBtn, "Apply circular averaging filter");
    addTooltip(coneBtn, "Apply cone-shaped weighted filter");
    addTooltip(laplacianBtn, "Apply Laplacian edge detection filter");
    addTooltip(sobelBtn, "Apply Sobel edge detection filter");
    
    connect(traditionalBtn, &QPushButton::clicked, this, &MainWindow::applyTraditionalFilter);
    connect(pyramidalBtn, &QPushButton::clicked, this, &MainWindow::applyPyramidalFilter);
    connect(circularBtn, &QPushButton::clicked, this, &MainWindow::applyCircularFilter);
    connect(coneBtn, &QPushButton::clicked, this, &MainWindow::applyConeFilter);
    connect(laplacianBtn, &QPushButton::clicked, this, &MainWindow::applyLaplacianFilter);
    connect(sobelBtn, &QPushButton::clicked, this, &MainWindow::applySobelFilter);
    
    filtersLayout->addWidget(traditionalBtn, 0, 0);
    filtersLayout->addWidget(pyramidalBtn, 0, 1);
    filtersLayout->addWidget(circularBtn, 1, 0);
    filtersLayout->addWidget(coneBtn, 1, 1);
    filtersLayout->addWidget(laplacianBtn, 2, 0);
    filtersLayout->addWidget(sobelBtn, 2, 1);
    
    // Color Processing Group (Lab 8)
    colorGroup = new QGroupBox("Lab 8: Color Processing");
    QGridLayout *colorLayout = new QGridLayout(colorGroup);
    
    QPushButton *colorSpaceBtn = new QPushButton("Color Space");
    QPushButton *splitChannelsBtn = new QPushButton("Split Channels");
    QPushButton *adjustColorsBtn = new QPushButton("Adjust Colors");
    QPushButton *whiteBalanceBtn = new QPushButton("White Balance");
    QPushButton *sepiaBtn = new QPushButton("Sepia Effect");
    QPushButton *coolFilterBtn = new QPushButton("Cool Filter");
    QPushButton *warmFilterBtn = new QPushButton("Warm Filter");
    QPushButton *vintageBtn = new QPushButton("Vintage Effect");
    
    addTooltip(colorSpaceBtn, "Convert to different color spaces (HSV, LAB, YCrCb)");
    addTooltip(splitChannelsBtn, "Split and visualize RGB channels separately");
    addTooltip(adjustColorsBtn, "Adjust brightness, contrast, saturation, and hue");
    addTooltip(whiteBalanceBtn, "Automatic white balance correction");
    addTooltip(sepiaBtn, "Apply sepia tone effect");
    addTooltip(coolFilterBtn, "Apply cool color filter (blue tint)");
    addTooltip(warmFilterBtn, "Apply warm color filter (orange tint)");
    addTooltip(vintageBtn, "Apply vintage/retro color effect");
    
    connect(colorSpaceBtn, &QPushButton::clicked, this, &MainWindow::convertToColorSpace);
    connect(splitChannelsBtn, &QPushButton::clicked, this, &MainWindow::splitRGBChannels);
    connect(adjustColorsBtn, &QPushButton::clicked, this, &MainWindow::adjustColors);
    connect(whiteBalanceBtn, &QPushButton::clicked, this, &MainWindow::applyWhiteBalance);
    connect(sepiaBtn, &QPushButton::clicked, this, &MainWindow::applySepiaEffect);
    connect(coolFilterBtn, &QPushButton::clicked, this, &MainWindow::applyCoolFilter);
    connect(warmFilterBtn, &QPushButton::clicked, this, &MainWindow::applyWarmFilter);
    connect(vintageBtn, &QPushButton::clicked, this, &MainWindow::applyVintageEffect);
    
    colorLayout->addWidget(colorSpaceBtn, 0, 0);
    colorLayout->addWidget(splitChannelsBtn, 0, 1);
    colorLayout->addWidget(adjustColorsBtn, 1, 0);
    colorLayout->addWidget(whiteBalanceBtn, 1, 1);
    colorLayout->addWidget(sepiaBtn, 2, 0);
    colorLayout->addWidget(coolFilterBtn, 2, 1);
    colorLayout->addWidget(warmFilterBtn, 3, 0);
    colorLayout->addWidget(vintageBtn, 3, 1);
    
    // Add groups to control layout
    controlLayout->addWidget(infoGroup);
    controlLayout->addWidget(transformGroup);
    controlLayout->addWidget(histogramGroup);
    controlLayout->addWidget(processingGroup);
    controlLayout->addWidget(filtersGroup);
    controlLayout->addWidget(colorGroup);
    controlLayout->addStretch();
    
    // Add scroll area to splitter
    mainSplitter->addWidget(scrollArea);
    
    // Set splitter sizes
    mainSplitter->setSizes({1000, 300}); // 3:1 ratio
    
    // Add splitter to main layout
    mainLayout->addWidget(mainSplitter);
}

void MainWindow::createStatusBar() {
    statusLabel = new QLabel("Ready - Load an image to get started");
    statusLabel->setStyleSheet("color: #f8f9fc;");
    
    progressBar = new QProgressBar();
    progressBar->setVisible(false);
    progressBar->setMaximumWidth(200);
    
    statusBar()->addWidget(statusLabel);
    statusBar()->addPermanentWidget(progressBar);
    statusBar()->setStyleSheet("QStatusBar { border-top: 1px solid #3a4a6f; }");
}

void MainWindow::addTooltip(QWidget *widget, const QString& text) {
    widget->setToolTip(text);
    widget->setToolTipDuration(3000);
}

QPixmap MainWindow::cvMatToQPixmap(const cv::Mat& mat) {
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

cv::Mat MainWindow::qPixmapToCvMat(const QPixmap& pixmap) {
    QImage qImg = pixmap.toImage().convertToFormat(QImage::Format_RGB888);
    cv::Mat mat(qImg.height(), qImg.width(), CV_8UC3, (void*)qImg.constBits(), qImg.bytesPerLine());
    cv::Mat result;
    cv::cvtColor(mat, result, cv::COLOR_RGB2BGR);
    return result.clone();
}

void MainWindow::updateDisplay() {
    if (!imageLoaded) return;
    
    originalCanvas->setImage(currentImage);
    
    QString originalInfo = QString("Size: %1x%2 | Channels: %3 | Type: %4")
                          .arg(currentImage.cols)
                          .arg(currentImage.rows)
                          .arg(currentImage.channels())
                          .arg(QString::fromStdString(cv::typeToString(currentImage.type())));
    originalInfoLabel->setText(originalInfo);
    
    if (!processedImage.empty()) {
        processedCanvas->setImage(processedImage);
        QString processedInfo = QString("Size: %1x%2 | Channels: %3 | Type: %4")
                               .arg(processedImage.cols)
                               .arg(processedImage.rows)
                               .arg(processedImage.channels())
                               .arg(QString::fromStdString(cv::typeToString(processedImage.type())));
        processedInfoLabel->setText(processedInfo);
        
        saveAction->setEnabled(true);
    } else {
        processedCanvas->clear();
        processedInfoLabel->setText("No processing applied");
        saveAction->setEnabled(false);
    }
}

void MainWindow::updateStatus(const QString& message, const QString& type, int progress) {
    statusLabel->setText(message);
    
    if (progress >= 0) {
        progressBar->setValue(progress);
        progressBar->setVisible(true);
    } else {
        progressBar->setVisible(false);
    }
    
    QApplication::processEvents();
}

// File operations implementation continues in next part...

void MainWindow::loadImage() {
    QString fileName = QFileDialog::getOpenFileName(this,
        "?? Load Image File",
        QString(),
        "Image Files (*.png *.jpg *.jpeg *.bmp *.tiff *.gif);;All Files (*.*)");
    
    if (fileName.isEmpty()) return;
    
    updateStatus("Loading image...", "info", 25);
    
    originalImage = cv::imread(fileName.toStdString());
    
    if (originalImage.empty()) {
        QMessageBox::critical(this, "Error", 
                             "Failed to load image file!\n\nPlease check the file format and try again.");
        updateStatus("Failed to load image", "error");
        return;
    }
    
    updateStatus("Image loaded successfully", "success", 100);
    
    currentImage = originalImage.clone();
    processedImage = cv::Mat(); // Clear processed image
    imagePath = fileName;
    imageLoaded = true;
    recentlyProcessed = false;
    
    updateDisplay();
    updateStatus(QString("Image loaded: %1").arg(QFileInfo(fileName).fileName()), "success");
    
    // Enable actions
    saveAction->setEnabled(false); // Only enable when there's processed image
    resetAction->setEnabled(true);
    
    // Enable all control groups
    infoGroup->setEnabled(true);
    transformGroup->setEnabled(true);
    histogramGroup->setEnabled(true);
    processingGroup->setEnabled(true);
}

void MainWindow::saveImage() {
    if (processedImage.empty()) {
        QMessageBox::warning(this, "Warning", "No processed image to save!");
        return;
    }
    
    QString fileName = QFileDialog::getSaveFileName(this,
        "?? Save Processed Image",
        QString(),
        "PNG Files (*.png);;JPEG Files (*.jpg);;BMP Files (*.bmp);;All Files (*.*)");
    
    if (fileName.isEmpty()) return;
    
    updateStatus("Saving image...", "info", 50);
    
    bool success = cv::imwrite(fileName.toStdString(), processedImage);
    
    if (success) {
        updateStatus("Image saved successfully", "success");
        QMessageBox::information(this, "Success", 
                                QString("Image saved successfully!\n\n%1").arg(fileName));
    } else {
        updateStatus("Failed to save image", "error");
        QMessageBox::critical(this, "Error", "Failed to save image file!");
    }
}

void MainWindow::resetImage() {
    if (!imageLoaded) return;
    
    currentImage = originalImage.clone();
    processedImage = cv::Mat();
    recentlyProcessed = false;
    
    updateDisplay();
    updateStatus("Image reset to original", "info");
}

// Lab 1: Image Information
void MainWindow::showImageInfo() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    QDialog *infoDialog = new QDialog(this);
    infoDialog->setWindowTitle("Image Information");
    infoDialog->setModal(true);
    infoDialog->setMinimumSize(500, 400);
    infoDialog->setStyleSheet("QDialog { background-color: #0a0e27; }");
    
    QVBoxLayout *layout = new QVBoxLayout(infoDialog);
    
    // Title
    QLabel *titleLabel = new QLabel("Detailed Image Information");
    titleLabel->setStyleSheet("font-size: 14pt; font-weight: bold; "
                             "color: #00d4ff; padding: 15px;");
    layout->addWidget(titleLabel);
    
    // Info text area
    QTextEdit *infoText = new QTextEdit();
    infoText->setReadOnly(true);
    infoText->setStyleSheet(
        "QTextEdit { "
        "background-color: #0f1535; "
        "color: #f8f9fc; "
        "border: 1px solid #3a4a6f; "
        "border-radius: 6px; "
        "padding: 15px; "
        "font-family: 'Consolas', monospace; "
        "font-size: 11pt; "
        "}"
    );
    
    // Build information string
    QString info;
    int rows = currentImage.rows;
    int cols = currentImage.cols;
    int channels = currentImage.channels();
    
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
    cv::minMaxLoc(currentImage, &minVal, &maxVal);
    meanVal = cv::mean(currentImage)[0];
    
    info += "???????????????????????????????????????????\n\n";
    info += QString("  File Path:               %1\n\n").arg(imagePath);
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
               cv::typeToString(currentImage.type())), 20);
    info += QString("  Min Value:               %1\n")
           .arg(minVal, 20, 'f', 2);
    info += QString("  Max Value:               %1\n")
           .arg(maxVal, 20, 'f', 2);
    info += QString("  Mean Value:              %1\n\n")
           .arg(meanVal, 20, 'f', 2);
    info += "???????????????????????????????????????????";
    
    infoText->setPlainText(info);
    layout->addWidget(infoText);
    
    // Close button
    QPushButton *closeBtn = new QPushButton("Close");
    closeBtn->setProperty("class", "accent");
    closeBtn->setMaximumWidth(150);
    connect(closeBtn, &QPushButton::clicked, infoDialog, &QDialog::accept);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(closeBtn);
    layout->addLayout(btnLayout);
    
    infoDialog->exec();
}

// Lab 2: Pixel Information
void MainWindow::showPixelInfo() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    QDialog *pixelDialog = new QDialog(this);
    pixelDialog->setWindowTitle("Pixel Information");
    pixelDialog->setModal(true);
    pixelDialog->setFixedSize(400, 220);
    pixelDialog->setStyleSheet("QDialog { background-color: #0a0e27; }");
    
    QVBoxLayout *layout = new QVBoxLayout(pixelDialog);
    
    // Title
    QLabel *titleLabel = new QLabel("Get Pixel Value");
    titleLabel->setStyleSheet("font-size: 12pt; font-weight: bold; "
                             "color: #00d4ff; padding: 10px;");
    layout->addWidget(titleLabel);
    
    // X coordinate input
    QHBoxLayout *xLayout = new QHBoxLayout();
    xLayout->addWidget(new QLabel(QString("Enter X coordinate (0-%1):")
                                 .arg(currentImage.cols - 1)));
    QSpinBox *xSpinBox = new QSpinBox();
    xSpinBox->setRange(0, currentImage.cols - 1);
    xSpinBox->setValue(0);
    xLayout->addWidget(xSpinBox);
    layout->addLayout(xLayout);
    
    // Y coordinate input
    QHBoxLayout *yLayout = new QHBoxLayout();
    yLayout->addWidget(new QLabel(QString("Enter Y coordinate (0-%1):")
                                 .arg(currentImage.rows - 1)));
    QSpinBox *ySpinBox = new QSpinBox();
    ySpinBox->setRange(0, currentImage.rows - 1);
    ySpinBox->setValue(0);
    yLayout->addWidget(ySpinBox);
    layout->addLayout(yLayout);
    
    layout->addSpacing(20);
    
    // Get pixel button
    QPushButton *getBtn = new QPushButton("Get Pixel Value");
    getBtn->setProperty("class", "accent");
    
    connect(getBtn, &QPushButton::clicked, [=]() {
        int x = xSpinBox->value();
        int y = ySpinBox->value();
        
        QString valueStr;
        if (currentImage.channels() == 1) {
            uchar val = currentImage.at<uchar>(y, x);
            valueStr = QString::number(val);
        } else if (currentImage.channels() == 3) {
            cv::Vec3b val = currentImage.at<cv::Vec3b>(y, x);
            valueStr = QString("B:%1, G:%2, R:%3")
                      .arg(val[0]).arg(val[1]).arg(val[2]);
        } else if (currentImage.channels() == 4) {
            cv::Vec4b val = currentImage.at<cv::Vec4b>(y, x);
            valueStr = QString("B:%1, G:%2, R:%3, A:%4")
                      .arg(val[0]).arg(val[1]).arg(val[2]).arg(val[3]);
        }
        
        QMessageBox msgBox(pixelDialog);
        msgBox.setWindowTitle("Pixel Value");
        msgBox.setText(QString("Value at (%1, %2):\n%3")
                      .arg(x).arg(y).arg(valueStr));
        msgBox.setStyleSheet("QMessageBox { background-color: #0f1535; }");
        msgBox.exec();
        
        pixelDialog->accept();
    });
    
    layout->addWidget(getBtn);
    
    pixelDialog->exec();
}

// Lab 3: Image Statistics
void MainWindow::showImageStats() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    double minVal, maxVal;
    cv::minMaxLoc(currentImage, &minVal, &maxVal);
    
    cv::Scalar meanScalar = cv::mean(currentImage);
    double meanVal = meanScalar[0];
    
    // Calculate standard deviation
    cv::Mat meanMat, stdDevMat;
    cv::meanStdDev(currentImage, meanMat, stdDevMat);
    double stdDev = stdDevMat.at<double>(0, 0);
    
    QString stats = QString(
        "Image Statistics:\n\n"
        "x Min Value: %1\n"
        "x Max Value: %2\n"
        "x Mean Value: %3\n"
        "x Standard Deviation: %4\n"
        "x Dynamic Range: %5"
    ).arg(minVal).arg(maxVal).arg(meanVal, 0, 'f', 2)
     .arg(stdDev, 0, 'f', 2).arg(maxVal - minVal);
    
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Image Statistics");
    msgBox.setText(stats);
    msgBox.setStyleSheet(
        "QMessageBox { background-color: #0f1535; } "
        "QLabel { color: #f8f9fc; font-size: 11pt; }"
    );
    msgBox.exec();
}

// Lab 4: Geometric Transformations
void MainWindow::applyTranslation() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    TransformDialog *dialog = new TransformDialog(
        this, 
        TransformDialog::Translation, 
        currentImage
    );
    
    // Connect preview signal to update processed canvas
    connect(dialog, &TransformDialog::previewRequested,
            [this](const cv::Mat& preview) {
                processedImage = preview.clone();
                updateDisplay();
            });
    
    if (dialog->exec() == QDialog::Accepted) {
        processedImage = dialog->getResultImage();
        recentlyProcessed = true;
        updateDisplay();
        updateStatus("Image translated successfully", "success");
    }
    
    delete dialog;
}

void MainWindow::applyRotation() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    TransformDialog *dialog = new TransformDialog(
        this, 
        TransformDialog::Rotation, 
        currentImage
    );
    
    connect(dialog, &TransformDialog::previewRequested,
            [this](const cv::Mat& preview) {
                processedImage = preview.clone();
                updateDisplay();
            });
    
    if (dialog->exec() == QDialog::Accepted) {
        processedImage = dialog->getResultImage();
        recentlyProcessed = true;
        updateDisplay();
        updateStatus("Image rotated successfully", "success");
    }
    
    delete dialog;
}

void MainWindow::applySkew() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    cv::Mat img = currentImage.clone();
    int rows = img.rows;
    int cols = img.cols;
    
    cv::Point2f srcTri[3];
    cv::Point2f dstTri[3];
    
    srcTri[0] = cv::Point2f(0, 0);
    srcTri[1] = cv::Point2f(cols - 1, 0);
    srcTri[2] = cv::Point2f(0, rows - 1);
    
    dstTri[0] = cv::Point2f(0, 0);
    dstTri[1] = cv::Point2f(cols - 1, 0);
    dstTri[2] = cv::Point2f(100, rows - 1);
    
    cv::Mat warpMat = cv::getAffineTransform(srcTri, dstTri);
    cv::warpAffine(img, processedImage, warpMat, img.size());
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Image skewed successfully", "success");
}

void MainWindow::applyZoom() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    TransformDialog *dialog = new TransformDialog(
        this, 
        TransformDialog::Zoom, 
        currentImage
    );
    
    connect(dialog, &TransformDialog::previewRequested,
            [this](const cv::Mat& preview) {
                processedImage = preview.clone();
                updateDisplay();
            });
    
    if (dialog->exec() == QDialog::Accepted) {
        processedImage = dialog->getResultImage();
        recentlyProcessed = true;
        updateDisplay();
        updateStatus("Image zoomed successfully", "success");
    }
    
    delete dialog;
}

void MainWindow::applyFlipX() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    cv::flip(currentImage, processedImage, 0); // Flip around x-axis
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Image flipped horizontally", "success");
}

void MainWindow::applyFlipY() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    cv::flip(currentImage, processedImage, 1); // Flip around y-axis
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Image flipped vertically", "success");
}

void MainWindow::applyFlipXY() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    cv::flip(currentImage, processedImage, -1); // Flip both axes
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Image flipped both ways", "success");
}

// Lab 5: Histogram Operations
void MainWindow::showHistogram() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    QDialog *histDialog = new QDialog(this);
    histDialog->setWindowTitle("Image Histogram");
    histDialog->setMinimumSize(900, 650);
    histDialog->setStyleSheet("QDialog { background-color: #0a0e27; }");
    
    QVBoxLayout *layout = new QVBoxLayout(histDialog);
    
    // Title
    QLabel *titleLabel = new QLabel("Pixel Value Distribution");
    titleLabel->setStyleSheet("font-size: 14pt; font-weight: bold; "
                             "color: #00d4ff; padding: 15px;");
    layout->addWidget(titleLabel);
    
    // Histogram widget
    HistogramWidget *histWidget = new HistogramWidget(histDialog);
    histWidget->setImage(currentImage);
    layout->addWidget(histWidget);
    
    // Close button
    QPushButton *closeBtn = new QPushButton("Close");
    closeBtn->setProperty("class", "accent");
    closeBtn->setMaximumWidth(150);
    connect(closeBtn, &QPushButton::clicked, histDialog, &QDialog::accept);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(closeBtn);
    layout->addLayout(btnLayout);
    
    histDialog->exec();
}

void MainWindow::applyHistogramEqualization() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    cv::Mat gray;
    if (currentImage.channels() == 3) {
        // Convert to YCrCb for color images
        cv::Mat ycrcb;
        cv::cvtColor(currentImage, ycrcb, cv::COLOR_BGR2YCrCb);
        
        std::vector<cv::Mat> channels;
        cv::split(ycrcb, channels);
        cv::equalizeHist(channels[0], channels[0]);
        cv::merge(channels, ycrcb);
        
        cv::cvtColor(ycrcb, processedImage, cv::COLOR_YCrCb2BGR);
    } else {
        cv::equalizeHist(currentImage, processedImage);
    }
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Histogram equalization applied", "success");
}

void MainWindow::applyOtsuThresholding() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    cv::Mat gray;
    if (currentImage.channels() == 3) {
        cv::cvtColor(currentImage, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = currentImage.clone();
    }
    
    cv::threshold(gray, processedImage, 0, 255, 
                 cv::THRESH_BINARY | cv::THRESH_OTSU);
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Otsu's thresholding applied", "success");
}

// Lab 6: Basic Image Processing
void MainWindow::convertToGrayscale() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    if (currentImage.channels() == 3) {
        cv::cvtColor(currentImage, processedImage, cv::COLOR_BGR2GRAY);
    } else {
        processedImage = currentImage.clone();
    }
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Converted to grayscale", "success");
}

void MainWindow::applyBinaryThreshold() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    cv::Mat gray;
    if (currentImage.channels() == 3) {
        cv::cvtColor(currentImage, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = currentImage.clone();
    }
    
    cv::threshold(gray, processedImage, 128, 255, cv::THRESH_BINARY);
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Binary threshold applied", "success");
}

void MainWindow::applyGaussianBlur() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    cv::GaussianBlur(currentImage, processedImage, cv::Size(5, 5), 0);
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Gaussian blur applied", "success");
}

void MainWindow::applyEdgeDetection() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    cv::Mat gray;
    if (currentImage.channels() == 3) {
        cv::cvtColor(currentImage, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = currentImage.clone();
    }
    
    cv::Canny(gray, processedImage, 100, 200);
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Edge detection applied", "success");
}

void MainWindow::invertColors() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    processedImage = 255 - currentImage;
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Colors inverted", "success");
}

// Lab 7: Custom Filters Implementation
void MainWindow::applyTraditionalFilter() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    updateStatus("Applying traditional filter...", "info", 50);
    
    // Traditional averaging filter (5x5 kernel with equal weights)
    cv::Mat kernel = (cv::Mat_<float>(5, 5) << 
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1) / 25.0f;
    
    cv::filter2D(currentImage, processedImage, -1, kernel);
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Traditional filter applied successfully", "success");
    
    // Show info dialog
    QMessageBox::information(this, "Traditional Filter",
        "Traditional averaging filter applied!\n\n"
        "Kernel: 5x5 with equal weights\n"
        "Effect: Smooths image by averaging neighboring pixels\n"
        "Use case: Noise reduction, basic smoothing");
}

void MainWindow::applyPyramidalFilter() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    updateStatus("Applying pyramidal filter...", "info", 50);
    
    // Pyramidal filter with weights increasing toward center
    cv::Mat kernel = (cv::Mat_<float>(5, 5) << 
        1, 2, 3, 2, 1,
        2, 3, 4, 3, 2,
        3, 4, 6, 4, 3,
        2, 3, 4, 3, 2,
        1, 2, 3, 2, 1);
    
    // Normalize kernel
    kernel = kernel / cv::sum(kernel)[0];
    
    cv::filter2D(currentImage, processedImage, -1, kernel);
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Pyramidal filter applied successfully", "success");
    
    // Show info dialog
    QMessageBox::information(this, "Pyramidal Filter",
        "Pyramidal weighted filter applied!\n\n"
        "Kernel: 5x5 with pyramidal weight distribution\n"
        "Effect: Smooth blur with center emphasis\n"
        "Use case: Better edge preservation than traditional filter");
}

void MainWindow::applyCircularFilter() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    updateStatus("Applying circular filter...", "info", 50);
    
    // Circular filter - only pixels within radius get weighted
    cv::Mat kernel = cv::Mat::zeros(5, 5, CV_32F);
    cv::Point center(2, 2);
    float radius = 2.0f;
    
    // Calculate circular mask
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            float dist = std::sqrt(std::pow(i - center.y, 2) + std::pow(j - center.x, 2));
            if (dist <= radius) {
                kernel.at<float>(i, j) = 1.0f;
            }
        }
    }
    
    // Normalize kernel
    kernel = kernel / cv::sum(kernel)[0];
    
    cv::filter2D(currentImage, processedImage, -1, kernel);
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Circular filter applied successfully", "success");
    
    // Show info dialog
    QMessageBox::information(this, "Circular Filter",
        "Circular averaging filter applied!\n\n"
        "Kernel: 5x5 with circular mask\n"
        "Effect: Isotropic smoothing (direction-independent)\n"
        "Use case: Natural-looking blur, uniform smoothing");
}

void MainWindow::applyConeFilter() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    updateStatus("Applying cone filter...", "info", 50);
    
    // Cone filter - weights decrease linearly from center
    cv::Mat kernel = cv::Mat::zeros(5, 5, CV_32F);
    cv::Point center(2, 2);
    float maxDist = std::sqrt(2 * 2 * 2); // Maximum distance in 5x5 kernel
    
    // Calculate cone-shaped weights
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            float dist = std::sqrt(std::pow(i - center.y, 2) + std::pow(j - center.x, 2));
            // Linear decrease from center
            kernel.at<float>(i, j) = std::max(0.0f, maxDist - dist);
        }
    }
    
    // Normalize kernel
    kernel = kernel / cv::sum(kernel)[0];
    
    cv::filter2D(currentImage, processedImage, -1, kernel);
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Cone filter applied successfully", "success");
    
    // Show info dialog
    QMessageBox::information(this, "Cone Filter",
        "Cone-shaped weighted filter applied!\n\n"
        "Kernel: 5x5 with cone weight distribution\n"
        "Effect: Smooth gradient blur from center\n"
        "Use case: Gaussian-like blur with linear falloff");
}

void MainWindow::applyLaplacianFilter() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    updateStatus("Applying Laplacian filter...", "info", 50);
    
    // Laplacian filter kernel (3x3)
    cv::Mat kernel_L = (cv::Mat_<float>(3, 3) << 
        1, 1, 1,
        1, -8, 1,
        1, 1, 1);
    
    cv::Mat dst_Lap;
    cv::filter2D(currentImage, dst_Lap, CV_8UC1, kernel_L);
    
    // Normalize for better visualization
    cv::normalize(dst_Lap, processedImage, 0, 255, cv::NORM_MINMAX, CV_8U);
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Laplacian filter applied successfully", "success");
    
    // Show info dialog
    QMessageBox::information(this, "Laplacian Filter",
        "Laplacian edge detection filter applied!\n\n"
        "Kernel: 3x3 second derivative operator\n"
        "Effect: Detects edges in all directions\n"
        "Use case: Edge detection, image sharpening");
}

void MainWindow::applySobelFilter() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    updateStatus("Applying Sobel filter...", "info", 50);
    
    // Sobel filter kernels
    cv::Mat kernel_TH = (cv::Mat_<int>(3, 3) << 
        -1, -2, -1,
        0, 0, 0,
        1, 2, 1);
    
    cv::Mat kernel_TV = (cv::Mat_<int>(3, 3) << 
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1);
    
    cv::Mat kernel_Td = (cv::Mat_<int>(3, 3) << 
        2, 1, 0,
        1, 0, -1,
        0, -1, -2);
    
    cv::Mat dstS_H, dstS_V, dstS_D, dstS_HV, dstS_S;
    
    // Apply filters
    cv::filter2D(currentImage, dstS_H, CV_8UC1, kernel_TH);
    cv::filter2D(currentImage, dstS_V, CV_8UC1, kernel_TV);
    cv::filter2D(currentImage, dstS_D, CV_8UC1, kernel_Td);
    
    // Combine horizontal and vertical
    cv::addWeighted(dstS_H, 1, dstS_V, 1, 0, dstS_HV);
    cv::addWeighted(dstS_HV, 1, dstS_D, 1, 0, dstS_S);
    
    // Normalize for better visualization
    cv::normalize(dstS_S, processedImage, 0, 255, cv::NORM_MINMAX, CV_8U);
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Sobel filter applied successfully", "success");
    
    // Show info dialog
    QMessageBox::information(this, "Sobel Filter",
        "Sobel edge detection filter applied!\n\n"
        "Kernels: 3x3 horizontal, vertical, and diagonal\n"
        "Effect: Detects directional edges\n"
        "Use case: Edge detection, gradient computation");
}

// =============================================================================
// Lab 8: Color Space Operations & Channel Manipulation
// =============================================================================

void MainWindow::convertToColorSpace() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    if (currentImage.channels() != 3) {
        QMessageBox::warning(this, "Warning", "Color space conversion requires a color image!");
        return;
    }
    
    // Let user select color space
    QStringList colorSpaces;
    colorSpaces << "HSV (Hue, Saturation, Value)" 
                << "LAB (Perceptual color space)" 
                << "YCrCb (Luminance, Chrominance)"
                << "HSL (Hue, Saturation, Lightness)"
                << "XYZ (CIE XYZ color space)";
    
    bool ok;
    QString selection = QInputDialog::getItem(this, "Color Space Conversion",
                                             "Select target color space:",
                                             colorSpaces, 0, false, &ok);
    
    if (!ok || selection.isEmpty()) {
        return;
    }
    
    updateStatus("Converting color space...", "info", 50);
    
    ColorProcessingLib::ColorSpace targetSpace;
    QString spaceName;
    
    if (selection.startsWith("HSV")) {
        targetSpace = ColorProcessingLib::ColorSpace::HSV;
        spaceName = "HSV";
    } else if (selection.startsWith("LAB")) {
        targetSpace = ColorProcessingLib::ColorSpace::LAB;
        spaceName = "LAB";
    } else if (selection.startsWith("YCrCb")) {
        targetSpace = ColorProcessingLib::ColorSpace::YCrCb;
        spaceName = "YCrCb";
    } else if (selection.startsWith("HSL")) {
        targetSpace = ColorProcessingLib::ColorSpace::HSL;
        spaceName = "HSL";
    } else if (selection.startsWith("XYZ")) {
        targetSpace = ColorProcessingLib::ColorSpace::XYZ;
        spaceName = "XYZ";
    } else {
        return;
    }
    
    cv::Mat result;
    if (ColorProcessingLib::convertColorSpace(currentImage, result, targetSpace)) {
        processedImage = result;
        recentlyProcessed = true;
        updateDisplay();
        updateStatus(QString("Converted to %1 color space").arg(spaceName), "success");
        
        QMessageBox::information(this, "Color Space Conversion",
            QString("Image converted to %1 color space!\n\n"
                   "The three channels now represent:\n%2")
            .arg(spaceName)
            .arg(selection.section(" (", 1).section(")", 0, 0)));
    } else {
        updateStatus("Color space conversion failed", "error");
        QMessageBox::critical(this, "Error", "Failed to convert color space!");
    }
}

void MainWindow::splitRGBChannels() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    if (currentImage.channels() != 3) {
        QMessageBox::warning(this, "Warning", "Channel split requires a color image!");
        return;
    }
    
    // Let user select which channel to visualize
    QStringList channels;
    channels << "Blue Channel" << "Green Channel" << "Red Channel" << "All Channels (composite)";
    
    bool ok;
    QString selection = QInputDialog::getItem(this, "Split RGB Channels",
                                             "Select channel to visualize:",
                                             channels, 3, false, &ok);
    
    if (!ok || selection.isEmpty()) {
        return;
    }
    
    updateStatus("Splitting RGB channels...", "info", 50);
    
    std::vector<cv::Mat> channelImages;
    ColorProcessingLib::splitChannels(currentImage, channelImages);
    
    if (channelImages.size() != 3) {
        QMessageBox::critical(this, "Error", "Failed to split channels!");
        return;
    }
    
    if (selection == "All Channels (composite)") {
        // Create composite view showing all three channels
        cv::Mat blue, green, red;
        ColorProcessingLib::visualizeChannel(currentImage, blue, 0);
        ColorProcessingLib::visualizeChannel(currentImage, green, 1);
        ColorProcessingLib::visualizeChannel(currentImage, red, 2);
        
        // Resize to fit side by side
        int newWidth = currentImage.cols / 3;
        int newHeight = currentImage.rows;
        
        cv::Mat blueResized, greenResized, redResized;
        cv::resize(blue, blueResized, cv::Size(newWidth, newHeight));
        cv::resize(green, greenResized, cv::Size(newWidth, newHeight));
        cv::resize(red, redResized, cv::Size(newWidth, newHeight));
        
        // Concatenate horizontally
        cv::Mat composite;
        cv::hconcat(blueResized, greenResized, composite);
        cv::hconcat(composite, redResized, processedImage);
        
        recentlyProcessed = true;
        updateDisplay();
        updateStatus("RGB channels displayed (Blue | Green | Red)", "success");
    } else {
        // Visualize single channel
        int channelIndex = selection.startsWith("Blue") ? 0 : 
                          (selection.startsWith("Green") ? 1 : 2);
        
        ColorProcessingLib::visualizeChannel(currentImage, processedImage, channelIndex);
        
        recentlyProcessed = true;
        updateDisplay();
        updateStatus(QString("%1 visualized").arg(selection), "success");
    }
}

void MainWindow::adjustColors() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    updateStatus("Opening color adjustment dialog...", "info", 0);
    
    // Create and show color adjustment dialog
    ColorAdjustDialog dialog(currentImage, this);
    
    // Connect preview signal
    connect(&dialog, &ColorAdjustDialog::previewRequested, 
            [this](const cv::Mat& preview) {
        processedImage = preview.clone();
        updateDisplay();
    });
    
    if (dialog.exec() == QDialog::Accepted) {
        processedImage = dialog.getAdjustedImage();
        recentlyProcessed = true;
        updateDisplay();
        
        QString msg = QString("Colors adjusted (B:%1, C:%2, S:%3, H:%4)")
                     .arg(dialog.getBrightness())
                     .arg(dialog.getContrast(), 0, 'f', 2)
                     .arg(dialog.getSaturation())
                     .arg(dialog.getHue());
        updateStatus(msg, "success");
    } else {
        // Restore original if cancelled
        if (recentlyProcessed) {
            updateDisplay();
        }
        updateStatus("Color adjustment cancelled", "info");
    }
}

void MainWindow::applyWhiteBalance() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    if (currentImage.channels() != 3) {
        QMessageBox::warning(this, "Warning", "White balance requires a color image!");
        return;
    }
    
    updateStatus("Applying white balance...", "info", 50);
    
    ColorProcessingLib::whiteBalance(currentImage, processedImage);
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("White balance applied successfully", "success");
    
    QMessageBox::information(this, "White Balance",
        "Automatic white balance correction applied!\n\n"
        "Algorithm: Gray World assumption\n"
        "Effect: Corrects color cast by normalizing average RGB values\n"
        "Use case: Fix images with color temperature issues");
}

void MainWindow::applySepiaEffect() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    if (currentImage.channels() != 3) {
        QMessageBox::warning(this, "Warning", "Sepia effect requires a color image!");
        return;
    }
    
    // Ask for intensity
    bool ok;
    int intensity = QInputDialog::getInt(this, "Sepia Effect",
                                        "Effect intensity (0-100):",
                                        80, 0, 100, 10, &ok);
    
    if (!ok) {
        return;
    }
    
    updateStatus("Applying sepia effect...", "info", 50);
    
    double intensityFactor = intensity / 100.0;
    ColorProcessingLib::applySepiaEffect(currentImage, processedImage, intensityFactor);
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus(QString("Sepia effect applied (intensity: %1%)").arg(intensity), "success");
}

void MainWindow::applyCoolFilter() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    if (currentImage.channels() != 3) {
        QMessageBox::warning(this, "Warning", "Cool filter requires a color image!");
        return;
    }
    
    // Ask for intensity
    bool ok;
    int intensity = QInputDialog::getInt(this, "Cool Filter",
                                        "Filter intensity (0-100):",
                                        50, 0, 100, 10, &ok);
    
    if (!ok) {
        return;
    }
    
    updateStatus("Applying cool filter...", "info", 50);
    
    double intensityFactor = intensity / 100.0;
    ColorProcessingLib::applyCoolFilter(currentImage, processedImage, intensityFactor);
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus(QString("Cool filter applied (intensity: %1%)").arg(intensity), "success");
}

void MainWindow::applyWarmFilter() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    if (currentImage.channels() != 3) {
        QMessageBox::warning(this, "Warning", "Warm filter requires a color image!");
        return;
    }
    
    // Ask for intensity
    bool ok;
    int intensity = QInputDialog::getInt(this, "Warm Filter",
                                        "Filter intensity (0-100):",
                                        50, 0, 100, 10, &ok);
    
    if (!ok) {
        return;
    }
    
    updateStatus("Applying warm filter...", "info", 50);
    
    double intensityFactor = intensity / 100.0;
    ColorProcessingLib::applyWarmFilter(currentImage, processedImage, intensityFactor);
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus(QString("Warm filter applied (intensity: %1%)").arg(intensity), "success");
}

void MainWindow::applyVintageEffect() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    if (currentImage.channels() != 3) {
        QMessageBox::warning(this, "Warning", "Vintage effect requires a color image!");
        return;
    }
    
    updateStatus("Applying vintage effect...", "info", 50);
    
    ColorProcessingLib::applyVintageEffect(currentImage, processedImage);
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Vintage effect applied successfully", "success");
    
    QMessageBox::information(this, "Vintage Effect",
        "Vintage/retro effect applied!\n\n"
        "Includes:\n"
        "- Sepia toning\n"
        "- Reduced contrast\n"
        "- Vignette effect\n"
        "Use case: Create nostalgic, old-photo appearance");
}