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
    
    QPixmap cvMatToQPixmap(const cv::Mat& mat);
    cv::Mat qPixmapToCvMat(const QPixmap& pixmap);
    
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
    QAction *exitAction;
    
    // Processing controls
    QGroupBox *infoGroup;
    QGroupBox *transformGroup;
    QGroupBox *histogramGroup;
    QGroupBox *processingGroup;
    QGroupBox *filtersGroup;
    
    // Image data
    cv::Mat originalImage;
    cv::Mat currentImage;
    cv::Mat processedImage;
    QString imagePath;
    
    // Processing state
    bool imageLoaded;
    bool recentlyProcessed;
};

#endif // MAINWINDOW_H