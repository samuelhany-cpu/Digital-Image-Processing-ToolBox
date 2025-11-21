#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QPushButton>

class SimpleImageViewer : public QMainWindow {
    Q_OBJECT

public:
    SimpleImageViewer(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Simple Image Processor - Test Version");
        setMinimumSize(800, 600);
        
        // Apply dark theme
        setStyleSheet(
            "QMainWindow { background-color: #0a0e27; }"
            "QWidget { background-color: #0f1535; color: #f8f9fc; }"
            "QPushButton { "
                "background-color: #1a2351; color: #f8f9fc; "
                "border: none; border-radius: 6px; padding: 12px; "
            "}"
            "QPushButton:hover { background-color: #00d4ff; color: #0a0e27; }"
            "QLabel { background-color: transparent; color: #f8f9fc; }"
        );
        
        setupUI();
    }

private slots:
    void showAbout() {
        QMessageBox::information(this, "About", 
            "Simple Image Processor Test\n\n"
            "This is a simplified version to test Qt6 functionality.\n"
            "If this works, we can debug the full version.");
    }
    
    void testOpenCV() {
        QMessageBox::information(this, "OpenCV Test", 
            "OpenCV testing functionality would go here.\n"
            "This version focuses on Qt6 GUI testing first.");
    }

private:
    void setupUI() {
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);
        
        // Title
        QLabel *titleLabel = new QLabel("?? Mexo Toolbox - Test Version");
        titleLabel->setStyleSheet("font-size: 18pt; font-weight: bold; color: #00d4ff; padding: 20px;");
        titleLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(titleLabel);
        
        // Info
        QLabel *infoLabel = new QLabel(
            "If you can see this window with the dark theme,\n"
            "Qt6 is working correctly and we can proceed to debug\n"
            "the full application."
        );
        infoLabel->setStyleSheet("font-size: 12pt; padding: 20px;");
        infoLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(infoLabel);
        
        // Buttons
        QHBoxLayout *btnLayout = new QHBoxLayout();
        
        QPushButton *aboutBtn = new QPushButton("About");
        QPushButton *testBtn = new QPushButton("Test OpenCV");
        QPushButton *exitBtn = new QPushButton("Exit");
        
        connect(aboutBtn, &QPushButton::clicked, this, &SimpleImageViewer::showAbout);
        connect(testBtn, &QPushButton::clicked, this, &SimpleImageViewer::testOpenCV);
        connect(exitBtn, &QPushButton::clicked, this, &QWidget::close);
        
        btnLayout->addWidget(aboutBtn);
        btnLayout->addWidget(testBtn);
        btnLayout->addWidget(exitBtn);
        
        layout->addLayout(btnLayout);
        layout->addStretch();
        
        // Menu bar
        QMenuBar *menuBar = this->menuBar();
        QMenu *fileMenu = menuBar->addMenu("File");
        fileMenu->addAction("About", this, &SimpleImageViewer::showAbout);
        fileMenu->addSeparator();
        fileMenu->addAction("Exit", this, &QWidget::close);
    }
};

#include "simple_test.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    app.setApplicationName("Simple Image Processor Test");
    app.setApplicationVersion("1.0");
    
    SimpleImageViewer window;
    window.show();
    
    return app.exec();
}