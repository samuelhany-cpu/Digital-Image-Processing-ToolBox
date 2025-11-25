#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

class ImageProcessorConsole {
private:
    Mat originalImage;
    Mat currentImage;
    Mat processedImage;
    string imagePath;
    bool imageLoaded;

public:
    ImageProcessorConsole() : imageLoaded(false) {}
    
    // Lab 1: Image Information
    void showImageInfo() {
        if (!imageLoaded) {
            cout << "? Please load an image first!" << endl;
            return;
        }
        
        cout << "\n?? IMAGE INFORMATION" << endl;
        cout << "????????????????????" << endl;
        cout << "File Path: " << imagePath << endl;
        cout << "Dimensions: " << currentImage.cols << " x " << currentImage.rows << " pixels" << endl;
        cout << "Channels: " << currentImage.channels() << endl;
        cout << "Data Type: " << typeToString(currentImage.type()) << endl;
        cout << "Total Pixels: " << (currentImage.rows * currentImage.cols) << endl;
        
        double minVal, maxVal;
        minMaxLoc(currentImage, &minVal, &maxVal);
        Scalar meanVal = mean(currentImage);
        
        cout << "Min Value: " << minVal << endl;
        cout << "Max Value: " << maxVal << endl;
        cout << "Mean Value: " << meanVal[0] << endl;
        cout << "Dynamic Range: " << (maxVal - minVal) << endl;
    }
    
    // Lab 2: Pixel Information
    void showPixelInfo() {
        if (!imageLoaded) {
            cout << "? Please load an image first!" << endl;
            return;
        }
        
        int x, y;
        cout << "\n?? PIXEL INFORMATION" << endl;
        cout << "???????????????????" << endl;
        cout << "Enter X coordinate (0-" << (currentImage.cols - 1) << "): ";
        cin >> x;
        cout << "Enter Y coordinate (0-" << (currentImage.rows - 1) << "): ";
        cin >> y;
        
        if (x < 0 || x >= currentImage.cols || y < 0 || y >= currentImage.rows) {
            cout << "? Invalid coordinates!" << endl;
            return;
        }
        
        cout << "Pixel value at (" << x << ", " << y << "): ";
        if (currentImage.channels() == 1) {
            uchar val = currentImage.at<uchar>(y, x);
            cout << (int)val << endl;
        } else if (currentImage.channels() == 3) {
            Vec3b val = currentImage.at<Vec3b>(y, x);
            cout << "B:" << (int)val[0] << ", G:" << (int)val[1] << ", R:" << (int)val[2] << endl;
        }
    }
    
    // Lab 3: Image Statistics
    void showImageStats() {
        if (!imageLoaded) {
            cout << "? Please load an image first!" << endl;
            return;
        }
        
        cout << "\n?? IMAGE STATISTICS" << endl;
        cout << "??????????????????" << endl;
        
        double minVal, maxVal;
        minMaxLoc(currentImage, &minVal, &maxVal);
        
        Mat meanMat, stdDevMat;
        meanStdDev(currentImage, meanMat, stdDevMat);
        
        cout << "Min Value: " << minVal << endl;
        cout << "Max Value: " << maxVal << endl;
        cout << "Mean Value: " << meanMat.at<double>(0, 0) << endl;
        cout << "Standard Deviation: " << stdDevMat.at<double>(0, 0) << endl;
        cout << "Dynamic Range: " << (maxVal - minVal) << endl;
    }
    
    // Lab 4: Geometric Transformations
    void applyTranslation() {
        if (!imageLoaded) {
            cout << "? Please load an image first!" << endl;
            return;
        }
        
        int tx, ty;
        cout << "\n? TRANSLATION" << endl;
        cout << "?????????????" << endl;
        cout << "Enter X translation (pixels): ";
        cin >> tx;
        cout << "Enter Y translation (pixels): ";
        cin >> ty;
        
        Mat M = (Mat_<float>(2, 3) << 1, 0, tx, 0, 1, ty);
        warpAffine(currentImage, processedImage, M, currentImage.size());
        
        cout << "? Translation applied successfully!" << endl;
        showProcessedImageOption();
    }
    
    void applyRotation() {
        if (!imageLoaded) {
            cout << "? Please load an image first!" << endl;
            return;
        }
        
        double angle;
        cout << "\n?? ROTATION" << endl;
        cout << "??????????" << endl;
        cout << "Enter rotation angle (degrees): ";
        cin >> angle;
        
        Point2f center(currentImage.cols / 2.0, currentImage.rows / 2.0);
        Mat M = getRotationMatrix2D(center, angle, 1.0);
        warpAffine(currentImage, processedImage, M, currentImage.size());
        
        cout << "? Rotation applied successfully!" << endl;
        showProcessedImageOption();
    }
    
    // Lab 5: Histogram Operations
    void applyHistogramEqualization() {
        if (!imageLoaded) {
            cout << "? Please load an image first!" << endl;
            return;
        }
        
        cout << "\n? HISTOGRAM EQUALIZATION" << endl;
        cout << "????????????????????????" << endl;
        
        if (currentImage.channels() == 3) {
            Mat ycrcb;
            cvtColor(currentImage, ycrcb, COLOR_BGR2YCrCb);
            vector<Mat> channels;
            split(ycrcb, channels);
            equalizeHist(channels[0], channels[0]);
            merge(channels, ycrcb);
            cvtColor(ycrcb, processedImage, COLOR_YCrCb2BGR);
        } else {
            equalizeHist(currentImage, processedImage);
        }
        
        cout << "Histogram equalization applied!" << endl;
        showProcessedImageOption();
    }
    
    // Lab 6: Image Processing
    void convertToGrayscale() {
        if (!imageLoaded) {
            cout << "? Please load an image first!" << endl;
            return;
        }
        
        cout << "\n? GRAYSCALE CONVERSION" << endl;
        cout << "?????????????????????" << endl;
        
        if (currentImage.channels() == 3) {
            cvtColor(currentImage, processedImage, COLOR_BGR2GRAY);
            cout << "Converted to grayscale!" << endl;
        } else {
            processedImage = currentImage.clone();
            cout << "?? Image is already grayscale!" << endl;
        }
        
        showProcessedImageOption();
    }
    
    void applyGaussianBlur() {
        if (!imageLoaded) {
            cout << "? Please load an image first!" << endl;
            return;
        }
        
        cout << "\n?? GAUSSIAN BLUR" << endl;
        cout << "???????????????" << endl;
        
        GaussianBlur(currentImage, processedImage, Size(15, 15), 0);
        
        cout << "Gaussian blur applied!" << endl;
        showProcessedImageOption();
    }
    
    void applyEdgeDetection() {
        if (!imageLoaded) {
            cout << "? Please load an image first!" << endl;
            return;
        }
        
        cout << "\n?? EDGE DETECTION" << endl;
        cout << "????????????????" << endl;
        
        Mat gray;
        if (currentImage.channels() == 3) {
            cvtColor(currentImage, gray, COLOR_BGR2GRAY);
        } else {
            gray = currentImage.clone();
        }
        
        Canny(gray, processedImage, 100, 200);
        
        cout << "Edge detection applied!" << endl;
        showProcessedImageOption();
    }
    
    // File operations
    bool loadImage(const string& path) {
        originalImage = imread(path);
        if (originalImage.empty()) {
            cout << "Failed to load image: " << path << endl;
            return false;
        }
        
        currentImage = originalImage.clone();
        imagePath = path;
        imageLoaded = true;
        
        cout << "? Image loaded successfully: " << path << endl;
        return true;
    }
    
    void saveProcessedImage() {
        if (processedImage.empty()) {
            cout << "? No processed image to save!" << endl;
            return;
        }
        
        string outputPath;
        cout << "Enter output file path: ";
        cin >> outputPath;
        
        if (imwrite(outputPath, processedImage)) {
            cout << "Image saved successfully: " << outputPath << endl;
        } else {
            cout << "Failed to save image!" << endl;
        }
    }
    
    void showProcessedImageOption() {
        cout << "\nOptions:" << endl;
        cout << "1. View processed image (OpenCV window)" << endl;
        cout << "2. Save processed image" << endl;
        cout << "3. Continue with menu" << endl;
        cout << "Choose (1-3): ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1:
                if (!processedImage.empty()) {
                    imshow("Processed Image", processedImage);
                    imshow("Original Image", originalImage);
                    cout << "Press any key in image window to continue..." << endl;
                    waitKey(0);
                    destroyAllWindows();
                }
                break;
            case 2:
                saveProcessedImage();
                break;
            case 3:
            default:
                break;
        }
    }
    
    void resetImage() {
        if (!imageLoaded) {
            cout << "? No image loaded!" << endl;
            return;
        }
        
        currentImage = originalImage.clone();
        processedImage = Mat();
        cout << "? Image reset to original!" << endl;
    }
    
    void runMainMenu() {
        cout << "\n?? MEXO TOOLBOX - Console Version" << endl;
        cout << "????????????????????????????????" << endl;
        
        while (true) {
            cout << "\nMain Menu:" << endl;
            cout << "0. Load Image" << endl;
            cout << "1. Lab 1: Image Information" << endl;
            cout << "2. Lab 2: Pixel Information" << endl;
            cout << "3. Lab 3: Image Statistics" << endl;
            cout << "4. Lab 4: Translation" << endl;
            cout << "5. Lab 4: Rotation" << endl;
            cout << "6. Lab 5: Histogram Equalization" << endl;
            cout << "7. Lab 6: Convert to Grayscale" << endl;
            cout << "8. Lab 6: Gaussian Blur" << endl;
            cout << "9. Lab 6: Edge Detection" << endl;
            cout << "10. Reset Image" << endl;
            cout << "11. Save Processed Image" << endl;
            cout << "12. Exit" << endl;
            cout << "\nChoose option (0-12): ";
            
            int choice;
            cin >> choice;
            
            switch (choice) {
                case 0: {
                    string path;
                    cout << "Enter image path: ";
                    cin >> path;
                    loadImage(path);
                    break;
                }
                case 1: showImageInfo(); break;
                case 2: showPixelInfo(); break;
                case 3: showImageStats(); break;
                case 4: applyTranslation(); break;
                case 5: applyRotation(); break;
                case 6: applyHistogramEqualization(); break;
                case 7: convertToGrayscale(); break;
                case 8: applyGaussianBlur(); break;
                case 9: applyEdgeDetection(); break;
                case 10: resetImage(); break;
                case 11: saveProcessedImage(); break;
                case 12:
                    cout << "?? Goodbye!" << endl;
                    return;
                default:
                    cout << "? Invalid choice!" << endl;
                    break;
            }
        }
    }
};

int main() {
    ImageProcessorConsole app;
    app.runMainMenu();
    return 0;
}