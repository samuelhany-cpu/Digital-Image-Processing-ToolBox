#include "MainWindow.h"
#include "ImageCanvas.h"
#include "TransformDialog.h"
#include "HistogramWidget.h"
#include "filters/ImageFilters.h"
#include "processing/ImageProcessingLib.h"
#include "processing/TransformationsLib.h"
#include "utils/ImageUtils.h"
#include <QApplication>
#include <QSplitter>
#include <QScrollArea>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), imageLoaded(false), recentlyProcessed(false) {
    
    setWindowTitle("Toolbox - Professional Image Processing");
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
    
    // Add Undo Action
    undoAction = new QAction("Undo", this);
    undoAction->setShortcut(QKeySequence::Undo); // Ctrl+Z
    undoAction->setToolTip("Undo last operation");
    undoAction->setEnabled(false);
    connect(undoAction, &QAction::triggered, this, &MainWindow::undoLastOperation);
    std::cout << "[DEBUG] Undo action created and connected" << std::endl;
    
    exitAction = new QAction("Exit", this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    
    fileMenu->addAction(loadAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addAction(resetAction);
    fileMenu->addAction(undoAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    
    // Enhancement Menu
    QMenu *enhanceMenu = menuBar->addMenu("Enhancement");
    QAction *autoEnhanceAction = enhanceMenu->addAction("Auto Enhance");
    autoEnhanceAction->setShortcut(Qt::CTRL | Qt::Key_E);
    connect(autoEnhanceAction, &QAction::triggered, this, &MainWindow::autoEnhance);
    
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
    
    std::cout << "[DEBUG] Menu bar created successfully" << std::endl;
}

void MainWindow::createToolBar() {
    QToolBar *toolBar = addToolBar("Main");
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    
    toolBar->addAction(loadAction);
    toolBar->addSeparator();
    toolBar->addAction(saveAction);
    toolBar->addAction(resetAction);
    toolBar->addAction(undoAction);
    
    std::cout << "[DEBUG] Toolbar created with undo button" << std::endl;
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
    
    // Auto Enhancement Group
    QGroupBox *autoGroup = new QGroupBox("Auto Enhancement");
    QVBoxLayout *autoLayout = new QVBoxLayout(autoGroup);
    
    QPushButton *autoEnhanceBtn = new QPushButton("Auto Enhance");
    autoEnhanceBtn->setStyleSheet("QPushButton { background-color: #ff006e; font-weight: bold; }");
    addTooltip(autoEnhanceBtn, "Automatically enhance image using multiple algorithms");
    connect(autoEnhanceBtn, &QPushButton::clicked, this, &MainWindow::autoEnhance);
    autoLayout->addWidget(autoEnhanceBtn);
    
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
    
    // Add groups to control layout
    controlLayout->addWidget(autoGroup);
    controlLayout->addWidget(infoGroup);
    controlLayout->addWidget(transformGroup);
    controlLayout->addWidget(histogramGroup);
    controlLayout->addWidget(processingGroup);
    controlLayout->addWidget(filtersGroup);
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
        
        // Calculate quality metrics
        QString metricsText = getQualityMetrics();
        
        QString processedInfo = QString("Size: %1x%2 | Channels: %3\n%4")
                               .arg(processedImage.cols)
                               .arg(processedImage.rows)
                               .arg(processedImage.channels())
                               .arg(metricsText);
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
        "Load Image File",
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

    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this,
        "Save Processed Image",
        QString(),
        "PNG Files (*.png);;JPEG Files (*.jpg);;BMP Files (*.bmp);;TIFF Files (*.tif *.tiff);;All Files (*.*)",
        &selectedFilter);

    if (fileName.isEmpty()) return;

    // Determine desired extension from selected filter if user didn't provide one
    QFileInfo fi(fileName);
    if (fi.suffix().isEmpty()) {
        QString ext = ".png";
        if (selectedFilter.contains("TIFF", Qt::CaseInsensitive)) ext = ".tif";
        else if (selectedFilter.contains("JPEG", Qt::CaseInsensitive) || selectedFilter.contains("JPG", Qt::CaseInsensitive)) ext = ".jpg";
        else if (selectedFilter.contains("BMP", Qt::CaseInsensitive)) ext = ".bmp";

        fileName += ext;
    }

    updateStatus("Saving image...", "info", 50);

    bool success = cv::imwrite(fileName.toStdString(), processedImage);

    if (success) {
        updateStatus("Image saved successfully", "success");
        QMessageBox::information(this, "Success",
                                QString("Image saved successfully!\n\n%1").arg(fileName));
    } else {
        updateStatus("Failed to save image", "error");
        QMessageBox::critical(this, "Error", "Failed to save image file!\n\nEnsure OpenCV was built with TIFF support if saving as TIFF.");
    }
}

void MainWindow::resetImage() {
    if (!imageLoaded) return;
    
    currentImage = originalImage.clone();
    processedImage = cv::Mat();
    recentlyProcessed = false;
    processingHistory.clear();
    lastOperation = "";
    processingStack.clear();
    
    updateDisplay();
    updateStatus("Image reset to original", "info");
}

void MainWindow::undoLastOperation() {
    std::cout << "[DEBUG] ===== UNDO FUNCTION CALLED =====" << std::endl;
    std::cout << "[DEBUG] Image loaded: " << (imageLoaded ? "YES" : "NO") << std::endl;
    std::cout << "[DEBUG] Processing stack size: " << processingStack.size() << std::endl;
    std::cout << "[DEBUG] Processing history size: " << processingHistory.size() << std::endl;
    
    if (!imageLoaded) {
        std::cout << "[DEBUG] Undo failed: No image loaded" << std::endl;
        QMessageBox::warning(this, "Warning", "Please load an image first!");
        return;
    }
    
    if (processingStack.empty()) {
        std::cout << "[DEBUG] Undo failed: Processing stack is empty" << std::endl;
        QMessageBox::information(this, "Undo", "No operations to undo!");
        return;
    }
    
    std::cout << "[DEBUG] Restoring state from stack..." << std::endl;
    
    // Restore previous state
    processedImage = processingStack.back().clone();
    processingStack.pop_back();
    
    std::cout << "[DEBUG] State restored. New stack size: " << processingStack.size() << std::endl;
    
    if (!processingHistory.isEmpty()) {
        QString undoneOperation = processingHistory.last();
        processingHistory.removeLast();
        std::cout << "[DEBUG] Removed operation from history: " << undoneOperation.toStdString() << std::endl;
    }
    
    // Enable/disable undo action based on stack state
    if (undoAction) {
        undoAction->setEnabled(!processingStack.empty());
        std::cout << "[DEBUG] Undo action " << (processingStack.empty() ? "disabled" : "enabled") << std::endl;
    }
    
    updateDisplay();
    updateStatus("Undo: Last operation reverted", "info");
    std::cout << "[DEBUG] ===== UNDO COMPLETED =====" << std::endl;
}

// Helper function to save state before processing
void MainWindow::saveProcessingState() {
    std::cout << "[DEBUG] ===== SAVING PROCESSING STATE =====" << std::endl;
    
    // For the FIRST operation: save the currentImage (original)
    // For subsequent operations: save the processedImage (previous result)
    if (processedImage.empty()) {
        // First operation - save the original currentImage
        std::cout << "[DEBUG] First operation detected - saving original currentImage" << std::endl;
        processingStack.push_back(currentImage.clone());
        std::cout << "[DEBUG] Original image saved to stack. Stack size: " << processingStack.size() << std::endl;
    } else {
        // Subsequent operations - save the current processed result
        std::cout << "[DEBUG] Subsequent operation - saving current processedImage" << std::endl;
        processingStack.push_back(processedImage.clone());
        std::cout << "[DEBUG] Processed image saved to stack. Stack size: " << processingStack.size() << std::endl;
    }
    
    // Limit stack size
    if (processingStack.size() > static_cast<size_t>(maxHistorySize)) {
        processingStack.erase(processingStack.begin());
        std::cout << "[DEBUG] Stack size exceeded limit. Removed oldest state. New size: " << processingStack.size() << std::endl;
    }
    
    // Enable undo action
    if (undoAction) {
        undoAction->setEnabled(true);
        std::cout << "[DEBUG] Undo action enabled" << std::endl;
    }
    
    std::cout << "[DEBUG] ===== STATE SAVE COMPLETED =====" << std::endl;
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
        "} "
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
    
    info += "======================================\n\n";
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
    info += "======================================";
    
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
    
    // Use processed image if available, otherwise use current
    cv::Mat sourceImage = processedImage.empty() ? currentImage : processedImage;
    
    TransformDialog *dialog = new TransformDialog(
        this, 
        TransformDialog::Translation, 
        sourceImage
    );
    
    // Connect preview signal to update processed canvas
    connect(dialog, &TransformDialog::previewRequested,
            [this](const cv::Mat& preview) {
                processedImage = preview.clone();
                updateDisplay();
            });
    
    if (dialog->exec() == QDialog::Accepted) {
        // Save state before applying transformation
        saveProcessingState();
        
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
    
    // Use processed image if available, otherwise use current
    cv::Mat sourceImage = processedImage.empty() ? currentImage : processedImage;
    
    TransformDialog *dialog = new TransformDialog(
        this, 
        TransformDialog::Rotation, 
        sourceImage
    );
    
    connect(dialog, &TransformDialog::previewRequested,
            [this](const cv::Mat& preview) {
                processedImage = preview.clone();
                updateDisplay();
            });
    
    if (dialog->exec() == QDialog::Accepted) {
        // Save state before applying transformation
        saveProcessingState();
        
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
    
    // Save state before processing
    saveProcessingState();
    
    // Use processed image if available, otherwise use current
    cv::Mat img = processedImage.empty() ? currentImage.clone() : processedImage.clone();
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
    
    // Use processed image if available, otherwise use current
    cv::Mat sourceImage = processedImage.empty() ? currentImage : processedImage;
    
    TransformDialog *dialog = new TransformDialog(
        this, 
        TransformDialog::Zoom, 
        sourceImage
    );
    
    connect(dialog, &TransformDialog::previewRequested,
            [this](const cv::Mat& preview) {
                processedImage = preview.clone();
                updateDisplay();
            });
    
    if (dialog->exec() == QDialog::Accepted) {
        // Save state before applying transformation
        saveProcessingState();
        
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
    
    // Save state before processing
    saveProcessingState();
    
    // Use processed image if available, otherwise use current
    cv::Mat sourceImage = processedImage.empty() ? currentImage : processedImage;
    cv::flip(sourceImage, processedImage, 0); // Flip around x-axis
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Image flipped horizontally", "success");
}

void MainWindow::applyFlipY() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    // Save state before processing
    saveProcessingState();
    
    // Use processed image if available, otherwise use current
    cv::Mat sourceImage = processedImage.empty() ? currentImage : processedImage;
    cv::flip(sourceImage, processedImage, 1); // Flip around y-axis
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Image flipped vertically", "success");
}

void MainWindow::applyFlipXY() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    // Save state before processing
    saveProcessingState();
    
    // Use processed image if available, otherwise use current
    cv::Mat sourceImage = processedImage.empty() ? currentImage : processedImage;
    cv::flip(sourceImage, processedImage, -1); // Flip both axes
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
    
    // Title - indicate which image we're showing
    QString titleText = processedImage.empty() ? 
        "Pixel Value Distribution - Original Image" : 
        "Pixel Value Distribution - Processed Image";
    
    QLabel *titleLabel = new QLabel(titleText);
    titleLabel->setStyleSheet("font-size: 14pt; font-weight: bold; "
                             "color: #00d4ff; padding: 15px;");
    layout->addWidget(titleLabel);
    
    // Histogram widget - use processed image if available, otherwise current
    HistogramWidget *histWidget = new HistogramWidget(histDialog);
    cv::Mat imageToAnalyze = processedImage.empty() ? currentImage : processedImage;
    histWidget->setImage(imageToAnalyze);
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
    
    // Save state before processing
    saveProcessingState();
    
    // Use processed image if available, otherwise use current
    cv::Mat sourceImage = processedImage.empty() ? currentImage : processedImage;
    ImageProcessingLib::applyHistogramEqualization(sourceImage, processedImage);
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Histogram equalization applied", "success");
}

void MainWindow::applyOtsuThresholding() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    // Save state before processing
    saveProcessingState();
    
    // Use processed image if available, otherwise use current
    cv::Mat sourceImage = processedImage.empty() ? currentImage : processedImage;
    ImageProcessingLib::applyOtsuThresholding(sourceImage, processedImage);
    
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
    
    // Save state before processing
    saveProcessingState();
    
    // Continuous processing: use processed image if available
    cv::Mat sourceImage = processedImage.empty() ? currentImage : processedImage;
    
    ImageProcessingLib::convertToGrayscale(sourceImage, processedImage);
    
    processingHistory << "Grayscale";
    lastOperation = "Grayscale";
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Converted to grayscale", "success");
}

void MainWindow::applyBinaryThreshold() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    // Save state before processing
    saveProcessingState();
    
    // Use processed image if available, otherwise use current
    cv::Mat sourceImage = processedImage.empty() ? currentImage : processedImage;
    ImageProcessingLib::applyBinaryThreshold(sourceImage, processedImage, 128);
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Binary threshold applied", "success");
}

void MainWindow::applyGaussianBlur() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    // Save state before processing
    saveProcessingState();
    
    // Use processed image if available, otherwise use current
    cv::Mat sourceImage = processedImage.empty() ? currentImage : processedImage;
    ImageProcessingLib::applyGaussianBlur(sourceImage, processedImage, 5);
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Gaussian blur applied", "success");
}

void MainWindow::applyEdgeDetection() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    // Save state before processing
    saveProcessingState();
    
    // Use processed image if available, otherwise use current
    cv::Mat sourceImage = processedImage.empty() ? currentImage : processedImage;
    ImageProcessingLib::applyEdgeDetection(sourceImage, processedImage, 100, 200);
    
    recentlyProcessed = true;
    updateDisplay();
    updateStatus("Edge detection applied", "success");
}

void MainWindow::invertColors() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    // Save state before processing
    saveProcessingState();
    
    // Use processed image if available, otherwise use current
    cv::Mat sourceImage = processedImage.empty() ? currentImage : processedImage;
    ImageProcessingLib::invertColors(sourceImage, processedImage);
    
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
    
    // Save state before processing
    saveProcessingState();
    
    updateStatus("Applying traditional filter...", "info", 50);
    
    // Use processed image if available, otherwise use current
    cv::Mat sourceImage = processedImage.empty() ? currentImage : processedImage;
    ImageFilters::applyTraditionalFilter(sourceImage, processedImage, 5);
    
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
    
    // Save state before processing
    saveProcessingState();
    
    updateStatus("Applying pyramidal filter...", "info", 50);
    
    // Use processed image if available, otherwise use current
    cv::Mat sourceImage = processedImage.empty() ? currentImage : processedImage;
    ImageFilters::applyPyramidalFilter(sourceImage, processedImage);
    
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
    
    // Save state before processing
    saveProcessingState();
    
    updateStatus("Applying circular filter...", "info", 50);
    
    // Use processed image if available, otherwise use current
    cv::Mat sourceImage = processedImage.empty() ? currentImage : processedImage;
    ImageFilters::applyCircularFilter(sourceImage, processedImage, 2.0f);
    
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
    
    // Save state before processing
    saveProcessingState();
    
    updateStatus("Applying cone filter...", "info", 50);
    
    // Use processed image if available, otherwise use current
    cv::Mat sourceImage = processedImage.empty() ? currentImage : processedImage;
    ImageFilters::applyConeFilter(sourceImage, processedImage);
    
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
    
    // Save state before processing
    saveProcessingState();
    
    updateStatus("Applying Laplacian filter...", "info", 50);
    
    // Use processed image if available, otherwise use current
    cv::Mat sourceImage = processedImage.empty() ? currentImage : processedImage;
    ImageFilters::applyLaplacianFilter(sourceImage, processedImage);
    
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
    
    // Save state before processing
    saveProcessingState();
    
    updateStatus("Applying Sobel filter...", "info", 50);
    
    // Use processed image if available, otherwise use current
    cv::Mat sourceImage = processedImage.empty() ? currentImage : processedImage;
    ImageFilters::applySobelFilter(sourceImage, processedImage);
    
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

void MainWindow::autoEnhance() {
    if (!imageLoaded) {
        QMessageBox::critical(this, "Error", "Please load an image first!");
        return;
    }
    
    // Save state before processing
    saveProcessingState();
    
    updateStatus("Auto-enhancing image...", "info", 50);
    
    // Get source image (continuous processing support)
    cv::Mat sourceImage = processedImage.empty() ? currentImage : processedImage;
    
    cv::Mat enhancedImage;
    QStringList operations;
    
    // Call auto enhance with operation tracking
    ImageProcessingLib::applyAutoEnhance(sourceImage, enhancedImage, operations);
    
    processedImage = enhancedImage.clone();
    processingHistory.append(operations);
    lastOperation = "Auto Enhancement";
    recentlyProcessed = true;
    
    updateDisplay();
    
    // Update info label with operations applied
    QString opsApplied = operations.join(" > ");
    processedInfoLabel->setText(QString("Enhanced: %1").arg(opsApplied));
    
    updateStatus("Auto enhancement completed", "success");
    
    // Show detailed dialog
    QMessageBox::information(this, "Auto Enhancement Complete",
        QString("Auto Enhancement Applied!\n\nOperations performed:\n• %1\n\n"
                "Each subsequent operation will build on this result.")
                .arg(operations.join("\n• ")));
}

// ==================== Image Quality Metrics Implementation ====================

double MainWindow::calculateMSE(const cv::Mat& original, const cv::Mat& processed) {
    if (original.empty() || processed.empty()) return 0.0;
    if (original.size() != processed.size()) return 0.0;
    
    cv::Mat diff;
    cv::absdiff(original, processed, diff);
    diff.convertTo(diff, CV_32F);
    diff = diff.mul(diff);
    
    cv::Scalar s = cv::sum(diff);
    double sumSquaredError = s[0] + s[1] + s[2];
    
    double mse = sumSquaredError / (double)(original.channels() * original.total());
    return mse;
}

double MainWindow::calculateRMSE(const cv::Mat& original, const cv::Mat& processed) {
    double mse = calculateMSE(original, processed);
    return std::sqrt(mse);
}

double MainWindow::calculatePSNR(const cv::Mat& original, const cv::Mat& processed) {
    double mse = calculateMSE(original, processed);
    if (mse <= 1e-10) return 100.0; // Perfect match
    
    double maxPixelValue = 255.0;
    double psnr = 10.0 * std::log10((maxPixelValue * maxPixelValue) / mse);
    return psnr;
}

double MainWindow::calculateSNR(const cv::Mat& original, const cv::Mat& processed) {
    if (original.empty() || processed.empty()) return 0.0;
    if (original.size() != processed.size()) return 0.0;
    
    // Convert to float for calculations
    cv::Mat origFloat, procFloat;
    original.convertTo(origFloat, CV_32F);
    processed.convertTo(procFloat, CV_32F);
    
    // Calculate signal power (original image)
    cv::Mat origSquared = origFloat.mul(origFloat);
    cv::Scalar signalPower = cv::sum(origSquared);
    double totalSignalPower = signalPower[0] + signalPower[1] + signalPower[2];
    
    // Calculate noise power (difference between original and processed)
    cv::Mat noise;
    cv::absdiff(origFloat, procFloat, noise);
    cv::Mat noiseSquared = noise.mul(noise);
    cv::Scalar noisePower = cv::sum(noiseSquared);
    double totalNoisePower = noisePower[0] + noisePower[1] + noisePower[2];
    
    if (totalNoisePower <= 1e-10) return 100.0; // No noise
    
    double snr = 10.0 * std::log10(totalSignalPower / totalNoisePower);
    return snr;
}

QString MainWindow::getQualityMetrics() {
    if (processedImage.empty() || currentImage.empty()) {
        return "No metrics available";
    }
    
    // Ensure both images have the same size
    if (currentImage.size() != processedImage.size()) {
        return "Size mismatch - cannot calculate metrics";
    }
    
    double mse = calculateMSE(currentImage, processedImage);
    double rmse = calculateRMSE(currentImage, processedImage);
    double psnr = calculatePSNR(currentImage, processedImage);
    double snr = calculateSNR(currentImage, processedImage);
    
    QString metrics = QString("MSE: %1 | RMSE: %2 | PSNR: %3 dB | SNR: %4 dB")
                     .arg(mse, 0, 'f', 2)
                     .arg(rmse, 0, 'f', 2)
                     .arg(psnr, 0, 'f', 2)
                     .arg(snr, 0, 'f', 2);
    
    return metrics;
}