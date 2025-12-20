#ifndef COLORADJUSTDIALOG_H
#define COLORADJUSTDIALOG_H

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
#include <opencv2/opencv.hpp>

/**
 * @brief ColorAdjustDialog provides an interactive Qt dialog for color adjustments
 * 
 * This dialog allows users to adjust:
 * - Brightness (-100 to +100)
 * - Contrast (0.5 to 3.0)
 * - Saturation (0 to 200)
 * - Hue (0 to 360 degrees)
 * - Temperature (-100 to +100)
 * 
 * Features:
 * - Real-time preview with live updates
 * - Reset button to restore original values
 * - Apply button to confirm changes
 * - Visual feedback with value labels
 */
class ColorAdjustDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param originalImage Input image to adjust
     * @param parent Parent widget
     */
    explicit ColorAdjustDialog(const cv::Mat& originalImage, QWidget *parent = nullptr);
    
    /**
     * @brief Destructor
     */
    ~ColorAdjustDialog();
    
    /**
     * @brief Get the adjusted image result
     * @return Processed image with applied adjustments
     */
    cv::Mat getAdjustedImage() const;
    
    /**
     * @brief Get brightness adjustment value
     * @return Brightness value (-100 to +100)
     */
    int getBrightness() const { return brightnessValue; }
    
    /**
     * @brief Get contrast adjustment value
     * @return Contrast multiplier (0.5 to 3.0)
     */
    double getContrast() const { return contrastValue; }
    
    /**
     * @brief Get saturation adjustment value
     * @return Saturation percentage (0 to 200)
     */
    int getSaturation() const { return saturationValue; }
    
    /**
     * @brief Get hue shift value
     * @return Hue degrees (0 to 360)
     */
    int getHue() const { return hueValue; }
    
    /**
     * @brief Get temperature adjustment value
     * @return Temperature value (-100 to +100)
     */
    int getTemperature() const { return temperatureValue; }

signals:
    /**
     * @brief Emitted when user requests preview update
     * @param adjusted Adjusted image for preview
     */
    void previewRequested(const cv::Mat& adjusted);

private slots:
    /**
     * @brief Handle brightness slider change
     * @param value New brightness value
     */
    void onBrightnessChanged(int value);
    
    /**
     * @brief Handle contrast slider change
     * @param value New contrast value (scaled)
     */
    void onContrastChanged(int value);
    
    /**
     * @brief Handle saturation slider change
     * @param value New saturation value
     */
    void onSaturationChanged(int value);
    
    /**
     * @brief Handle hue slider change
     * @param value New hue value
     */
    void onHueChanged(int value);
    
    /**
     * @brief Handle temperature slider change
     * @param value New temperature value
     */
    void onTemperatureChanged(int value);
    
    /**
     * @brief Reset all adjustments to default values
     */
    void onResetClicked();
    
    /**
     * @brief Apply adjustments and close dialog
     */
    void onApplyClicked();
    
    /**
     * @brief Cancel adjustments and close dialog
     */
    void onCancelClicked();
    
    /**
     * @brief Update preview with current adjustment values
     */
    void updatePreview();

private:
    /**
     * @brief Setup the dialog UI components
     */
    void setupUI();
    
    /**
     * @brief Create slider control group
     * @param label Label text
     * @param min Minimum value
     * @param max Maximum value
     * @param defaultValue Default value
     * @param slider Output slider widget
     * @param valueLabel Output value display label
     * @return QGroupBox containing the control
     */
    QGroupBox* createSliderGroup(const QString& label, int min, int max, 
                                 int defaultValue, QSlider*& slider, 
                                 QLabel*& valueLabel);
    
    /**
     * @brief Apply stylesheet to dialog
     */
    void applyStyleSheet();
    
    /**
     * @brief Process image with current adjustment values
     * @return Processed image
     */
    cv::Mat processImage();
    
    // Original and result images
    cv::Mat originalImage;
    cv::Mat adjustedImage;
    
    // Adjustment values
    int brightnessValue;
    double contrastValue;
    int saturationValue;
    int hueValue;
    int temperatureValue;
    
    // UI Components - Sliders
    QSlider *brightnessSlider;
    QSlider *contrastSlider;
    QSlider *saturationSlider;
    QSlider *hueSlider;
    QSlider *temperatureSlider;
    
    // UI Components - Value labels
    QLabel *brightnessLabel;
    QLabel *contrastLabel;
    QLabel *saturationLabel;
    QLabel *hueLabel;
    QLabel *temperatureLabel;
    
    // UI Components - Buttons
    QPushButton *resetButton;
    QPushButton *applyButton;
    QPushButton *cancelButton;
    
    // UI Components - Preview option
    QCheckBox *livePreviewCheckBox;
    
    // State
    bool livePreviewEnabled;
};

#endif // COLORADJUSTDIALOG_H
