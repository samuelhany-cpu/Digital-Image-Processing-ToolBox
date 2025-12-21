#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QComboBox>
#include <opencv2/opencv.hpp>

/**
 * @brief FilterDialog provides an interactive Qt dialog for advanced filter adjustment
 * 
 * Supports configuration for:
 * - Median Filter (kernel size)
 * - Bilateral Filter (d, sigmaColor, sigmaSpace)
 * - Non-Local Means (h, template window, search window)
 * - Morphological operations (kernel size, shape)
 * - Unsharp Mask (sigma, amount, threshold)
 * - High-Pass Filter (kernel size)
 * - Custom Sharpen (strength)
 */
class FilterDialog : public QDialog {
    Q_OBJECT

public:
    enum FilterType {
        MEDIAN,
        BILATERAL,
        NON_LOCAL_MEANS,
        MORPHOLOGICAL_OPENING,
        MORPHOLOGICAL_CLOSING,
        MORPHOLOGICAL_GRADIENT,
        TOP_HAT,
        BLACK_HAT,
        UNSHARP_MASK,
        HIGH_PASS,
        CUSTOM_SHARPEN
    };

    /**
     * @brief Constructor
     * @param originalImage Input image to filter
     * @param filterType Type of filter to configure
     * @param parent Parent widget
     */
    explicit FilterDialog(const cv::Mat& originalImage, FilterType filterType, 
                         QWidget *parent = nullptr);
    
    /**
     * @brief Destructor
     */
    ~FilterDialog();
    
    /**
     * @brief Get the filtered image result
     * @return Processed image with applied filter
     */
    cv::Mat getFilteredImage() const;
    
    // Median Filter parameters
    int getMedianKernelSize() const { return medianKernelSize; }
    
    // Bilateral Filter parameters
    int getBilateralD() const { return bilateralD; }
    double getBilateralSigmaColor() const { return bilateralSigmaColor; }
    double getBilateralSigmaSpace() const { return bilateralSigmaSpace; }
    
    // Non-Local Means parameters
    float getNLMH() const { return nlmH; }
    int getNLMTemplateWindow() const { return nlmTemplateWindow; }
    int getNLMSearchWindow() const { return nlmSearchWindow; }
    
    // Morphological parameters
    int getMorphKernelSize() const { return morphKernelSize; }
    int getMorphKernelShape() const { return morphKernelShape; }
    
    // Unsharp Mask parameters
    double getUnsharpSigma() const { return unsharpSigma; }
    double getUnsharpAmount() const { return unsharpAmount; }
    int getUnsharpThreshold() const { return unsharpThreshold; }
    
    // High-Pass Filter parameters
    int getHighPassKernelSize() const { return highPassKernelSize; }
    
    // Custom Sharpen parameters
    int getSharpenStrength() const { return sharpenStrength; }

signals:
    /**
     * @brief Emitted when user requests preview update
     * @param filtered Filtered image for preview
     */
    void previewRequested(const cv::Mat& filtered);

private slots:
    void onParameterChanged();
    void onResetClicked();
    void onApplyClicked();
    void onCancelClicked();
    void updatePreview();

private:
    void setupUI();
    void setupMedianUI();
    void setupBilateralUI();
    void setupNLMUI();
    void setupMorphologicalUI();
    void setupUnsharpUI();
    void setupHighPassUI();
    void setupSharpenUI();
    
    QGroupBox* createSliderGroup(const QString& label, int min, int max, 
                                 int defaultValue, QSlider*& slider, 
                                 QLabel*& valueLabel);
    QGroupBox* createDoubleSliderGroup(const QString& label, double min, double max, 
                                      double defaultValue, double step,
                                      QSlider*& slider, QLabel*& valueLabel);
    
    void applyStyleSheet();
    cv::Mat processImage();
    
    // Original and result images
    cv::Mat originalImage;
    cv::Mat filteredImage;
    FilterType filterType;
    
    // Common parameters
    int medianKernelSize;
    int bilateralD;
    double bilateralSigmaColor;
    double bilateralSigmaSpace;
    float nlmH;
    int nlmTemplateWindow;
    int nlmSearchWindow;
    int morphKernelSize;
    int morphKernelShape;
    double unsharpSigma;
    double unsharpAmount;
    int unsharpThreshold;
    int highPassKernelSize;
    int sharpenStrength;
    
    // UI Components
    QVBoxLayout *mainLayout;
    QGroupBox *parametersGroup;
    
    // Sliders and labels (used based on filter type)
    QSlider *slider1;
    QSlider *slider2;
    QSlider *slider3;
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
    QComboBox *shapeCombo;
    
    // Buttons
    QPushButton *resetButton;
    QPushButton *applyButton;
    QPushButton *cancelButton;
    QCheckBox *livePreviewCheckBox;
    
    bool livePreviewEnabled;
};

#endif // FILTERDIALOG_H
