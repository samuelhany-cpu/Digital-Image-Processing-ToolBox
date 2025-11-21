#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QPushButton>
#include <opencv2/opencv.hpp>
#include <iostream>

class MinimalImageApp : public QMainWindow {
    Q_OBJECT

public:
    MinimalImageApp(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Minimal Image App - OpenCV Test");
        setMinimumSize(600, 400);
        
        // Apply basic dark theme
        setStyleSheet(
            "QMainWindow { background-color: #0a0e27; }"
            "QWidget { background-color: #0f1535; color: #f8f9fc; }"
            "QPushButton { background-color: #1a2351; color: #f8f9fc; border: none; border-radius: 6px; padding: 12px; }"
            "QPushButton:hover { background-color: #00d4ff; color: #0a0e27; }"
        );
        
        setupUI();
        
        // Test OpenCV initialization
        testOpenCV();
    }

private slots:
    void testOpenCVBasic() {
        try {
            std::cout << "Testing basic OpenCV functionality..." << std::endl;
            
            // Create a simple test image
            cv::Mat testImg = cv::Mat::zeros(100, 100, CV_8UC3);
            testImg.setTo(cv::Scalar(0, 255, 0)); // Green image
            
            std::cout << "OpenCV Mat created successfully" << std::endl;
            std::cout << "Image size: " << testImg.cols << "x" << testImg.rows << std::endl;
            std::cout << "Image channels: " << testImg.channels() << std::endl;
            
            QMessageBox::information(this, "OpenCV Test", 
                QString("OpenCV is working!\n\n"
                       "Created test image: %1x%2\n"
                       "Channels: %3")
                       .arg(testImg.cols).arg(testImg.rows).arg(testImg.channels()));
                       
        } catch (const std::exception& e) {
            std::cerr << "OpenCV test failed: " << e.what() << std::endl;
            QMessageBox::critical(this, "OpenCV Error", 
                QString("OpenCV test failed:\n%1").arg(e.what()));
        }
    }
    
    void testImageLoad() {
        try {
            std::cout << "Testing image loading..." << std::endl;
            
            // Try to load the test image
            cv::Mat img = cv::imread("translation.jpg");
            
            if (img.empty()) {
                QMessageBox::warning(this, "Image Load", 
                    "Could not load translation.jpg\n"
                    "Make sure the image file exists in the application directory.");
                return;
            }
            
            std::cout << "Image loaded successfully" << std::endl;
            std::cout << "Image size: " << img.cols << "x" << img.rows << std::endl;
            
            QMessageBox::information(this, "Image Load Test", 
                QString("Image loaded successfully!\n\n"
                       "Size: %1x%2\n"
                       "Channels: %3")
                       .arg(img.cols).arg(img.rows).arg(img.channels()));
                       
        } catch (const std::exception& e) {
            std::cerr << "Image load test failed: " << e.what() << std::endl;
            QMessageBox::critical(this, "Image Load Error", 
                QString("Image load failed:\n%1").arg(e.what()));
        }
    }

private:
    void setupUI() {
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);
        
        QLabel *titleLabel = new QLabel("?? Minimal OpenCV Test Application");
        titleLabel->setStyleSheet("font-size: 16pt; font-weight: bold; color: #00d4ff; padding: 20px;");
        titleLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(titleLabel);
        
        QLabel *infoLabel = new QLabel("This minimal version tests OpenCV functionality step by step.");
        infoLabel->setStyleSheet("font-size: 12pt; padding: 10px;");
        infoLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(infoLabel);
        
        QPushButton *testBasicBtn = new QPushButton("Test Basic OpenCV");
        QPushButton *testLoadBtn = new QPushButton("Test Image Loading");
        QPushButton *exitBtn = new QPushButton("Exit");
        
        connect(testBasicBtn, &QPushButton::clicked, this, &MinimalImageApp::testOpenCVBasic);
        connect(testLoadBtn, &QPushButton::clicked, this, &MinimalImageApp::testImageLoad);
        connect(exitBtn, &QPushButton::clicked, this, &QWidget::close);
        
        layout->addWidget(testBasicBtn);
        layout->addWidget(testLoadBtn);
        layout->addWidget(exitBtn);
        layout->addStretch();
    }
    
    void testOpenCV() {
        try {
            std::cout << "OpenCV version: " << CV_VERSION << std::endl;
            std::cout << "OpenCV initialization successful" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "OpenCV initialization failed: " << e.what() << std::endl;
        }
    }
};

#include "minimal_opencv_test.moc"

int main(int argc, char *argv[]) {
    try {
        QApplication app(argc, argv);
        
        std::cout << "Qt Application starting..." << std::endl;
        
        // Test OpenCV version first
        std::cout << "OpenCV version: " << CV_VERSION << std::endl;
        
        MinimalImageApp window;
        std::cout << "Main window created" << std::endl;
        
        window.show();
        std::cout << "Window shown, starting event loop" << std::endl;
        
        return app.exec();
        
    } catch (const std::exception& e) {
        std::cerr << "Exception in main: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Unknown exception in main" << std::endl;
        return -1;
    }
}