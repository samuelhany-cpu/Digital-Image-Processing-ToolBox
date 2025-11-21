#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <opencv2/opencv.hpp>
#include <iostream>

class SimpleMainWindow : public QMainWindow {
    Q_OBJECT

public:
    SimpleMainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        std::cout << "SimpleMainWindow constructor started" << std::endl;
        
        setWindowTitle("?? Mexo Toolbox - Simplified Version");
        setMinimumSize(1000, 700);
        
        std::cout << "Basic properties set" << std::endl;
        
        // Apply the same dark theme as the full app
        setStyleSheet(
            "QMainWindow { background-color: #0a0e27; }"
            "QWidget { background-color: #0f1535; color: #f8f9fc; font-family: 'Segoe UI', Arial; font-size: 10pt; }"
            "QPushButton { background-color: #1a2351; color: #f8f9fc; border: none; border-radius: 6px; padding: 12px; min-width: 120px; }"
            "QPushButton:hover { background-color: #252d48; color: #00d4ff; }"
            "QLabel { background-color: transparent; color: #f8f9fc; }"
            "QMenuBar { background-color: #0f1535; color: #f8f9fc; border-bottom: 1px solid #3a4a6f; }"
        );
        
        std::cout << "Stylesheet applied" << std::endl;
        
        setupUI();
        std::cout << "UI setup completed" << std::endl;
    }

private slots:
    void loadImage() {
        std::cout << "Load image clicked" << std::endl;
        
        QString fileName = QFileDialog::getOpenFileName(this,
            "Load Image File",
            QString(),
            "Image Files (*.png *.jpg *.jpeg *.bmp);;All Files (*.*)");
        
        if (fileName.isEmpty()) return;
        
        std::cout << "Loading: " << fileName.toStdString() << std::endl;
        
        cv::Mat image = cv::imread(fileName.toStdString());
        
        if (image.empty()) {
            QMessageBox::critical(this, "Error", "Failed to load image!");
            return;
        }
        
        QMessageBox::information(this, "Success", 
            QString("Image loaded successfully!\n\n"
                   "Size: %1x%2\n"
                   "Channels: %3")
                   .arg(image.cols).arg(image.rows).arg(image.channels()));
    }
    
    void showImageInfo() {
        std::cout << "Show image info clicked" << std::endl;
        QMessageBox::information(this, "Image Info", "Image information feature placeholder");
    }
    
    void showAbout() {
        std::cout << "About clicked" << std::endl;
        QMessageBox::information(this, "About", 
            "Mexo Toolbox - Simplified Version\n\n"
            "This version tests the core functionality step by step.");
    }

private:
    void setupUI() {
        std::cout << "Setting up UI..." << std::endl;
        
        // Central widget
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        std::cout << "Central widget created" << std::endl;
        
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
        
        // Title
        QLabel *titleLabel = new QLabel("?? Mexo Toolbox - Simplified Test Version");
        titleLabel->setStyleSheet("font-size: 16pt; font-weight: bold; color: #00d4ff; padding: 20px;");
        titleLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(titleLabel);
        std::cout << "Title label created" << std::endl;
        
        // Info
        QLabel *infoLabel = new QLabel(
            "This simplified version tests core functionality without complex widgets.\n"
            "If this works, we can identify what's causing the crash in the full version."
        );
        infoLabel->setStyleSheet("font-size: 12pt; padding: 20px;");
        infoLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(infoLabel);
        std::cout << "Info label created" << std::endl;
        
        // Buttons
        QVBoxLayout *btnLayout = new QVBoxLayout();
        
        QPushButton *loadBtn = new QPushButton("?? Load Image");
        QPushButton *infoBtn = new QPushButton("?? Show Image Info");
        QPushButton *aboutBtn = new QPushButton("?? About");
        
        connect(loadBtn, &QPushButton::clicked, this, &SimpleMainWindow::loadImage);
        connect(infoBtn, &QPushButton::clicked, this, &SimpleMainWindow::showImageInfo);
        connect(aboutBtn, &QPushButton::clicked, this, &SimpleMainWindow::showAbout);
        
        btnLayout->addWidget(loadBtn);
        btnLayout->addWidget(infoBtn);
        btnLayout->addWidget(aboutBtn);
        
        mainLayout->addLayout(btnLayout);
        mainLayout->addStretch();
        
        std::cout << "Buttons created and connected" << std::endl;
        
        // Simple menu bar
        QMenuBar *menuBar = this->menuBar();
        QMenu *fileMenu = menuBar->addMenu("File");
        fileMenu->addAction("Load Image", this, &SimpleMainWindow::loadImage);
        fileMenu->addAction("About", this, &SimpleMainWindow::showAbout);
        fileMenu->addSeparator();
        fileMenu->addAction("Exit", this, &QWidget::close);
        
        std::cout << "Menu bar created" << std::endl;
    }
};

#include "simplified_main.moc"

int main(int argc, char *argv[]) {
    try {
        std::cout << "Starting simplified application..." << std::endl;
        
        QApplication app(argc, argv);
        
        app.setApplicationName("Mexo Toolbox Simplified");
        app.setApplicationVersion("1.0");
        
        std::cout << "Creating simplified main window..." << std::endl;
        SimpleMainWindow window;
        
        std::cout << "Showing window..." << std::endl;
        window.show();
        
        std::cout << "Starting event loop..." << std::endl;
        return app.exec();
        
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Unknown exception" << std::endl;
        return -1;
    }
}