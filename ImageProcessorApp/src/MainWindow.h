#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QStatusBar>
#include <QMenuBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include <memory>

class ImageCanvas;
class HistogramWidget;
class ColorAdjustDialog;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // File operations
    void loadImage();
    void saveImage();
    void resetImage();
    void undoLastOperation();
    
    // Auto Enhancement
    void autoEnhance();
    
    // Lab 1: Image Information
    void showImageInfo();
    void showPixelInfo();
    void showImageStats();
    
    // Lab 4: Geometric Transformations
    void applyTranslation();
    void applyRotation();
    void applySkew();
    void applyZoom();
    void applyFlipX();
    void applyFlipY();
    void applyFlipXY();
    
    // Lab 5: Histogram & Thresholding
    void showHistogram();
    void applyHistogramEqualization();
    void applyOtsuThresholding();
    
    // Lab 6: Image Processing
    void convertToGrayscale();
    void applyBinaryThreshold();
    void applyGaussianBlur();
    void applyEdgeDetection();
    void invertColors();
    
    // Lab 7: Custom Filters
    void applyTraditionalFilter();
    void applyPyramidalFilter();
    void applyCircularFilter();
    void applyConeFilter();
    void applyLaplacianFilter();
    void applySobelFilter();
    
    // Lab 8: Color Space Operations
    void convertToColorSpace();
    void splitRGBChannels();
    void adjustColors();
    void applyWhiteBalance();
    void applySepiaEffect();
    void applyCoolFilter();
    void applyWarmFilter();
    void applyVintageEffect();

private:
    void setupUI();
    void createMenuBar();
    void createToolBar();
    void createCentralWidget();
    void createStatusBar();
    void createProcessingControls();
    void applyStyleSheet();
    
    void updateDisplay();
    void updateStatus(const QString& message, 
                     const QString& type = "info", 
                     int progress = -1);
    void addTooltip(QWidget *widget, const QString& text);
    void saveProcessingState();  // Save current state before processing
    
    QPixmap cvMatToQPixmap(const cv::Mat& mat);
    cv::Mat qPixmapToCvMat(const QPixmap& pixmap);
    
    // Image quality metrics
    double calculateMSE(const cv::Mat& original, const cv::Mat& processed);
    double calculateRMSE(const cv::Mat& original, const cv::Mat& processed);
    double calculatePSNR(const cv::Mat& original, const cv::Mat& processed);
    double calculateSNR(const cv::Mat& original, const cv::Mat& processed);
    QString getQualityMetrics();
    
    // UI Components
    ImageCanvas *originalCanvas;
    ImageCanvas *processedCanvas;
    
    QLabel *originalInfoLabel;
    QLabel *processedInfoLabel;
    QLabel *processedTitleLabel;
    QLabel *statusLabel;
    QProgressBar *progressBar;
    
    // Menu and toolbar actions
    QAction *loadAction;
    QAction *saveAction;
    QAction *resetAction;
    QAction *undoAction;
    QAction *exitAction;
    
    // Processing controls
    QGroupBox *infoGroup;
    QGroupBox *transformGroup;
    QGroupBox *histogramGroup;
    QGroupBox *processingGroup;
    QGroupBox *filtersGroup;
    QGroupBox *colorGroup;
    
    // Image data
    cv::Mat originalImage;
    cv::Mat currentImage;
    cv::Mat processedImage;
    QString imagePath;
    
    // Processing state
    bool imageLoaded;
    bool recentlyProcessed;
    
    // Processing history
    QStringList processingHistory;
    QString lastOperation;
    std::vector<cv::Mat> processingStack;  // Stack to store previous states
    int maxHistorySize = 10;  // Maximum undo steps
};

#endif // MAINWINDOW_H