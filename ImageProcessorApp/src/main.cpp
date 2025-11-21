#include <QApplication>
#include <QDir>
#include <QStyleFactory>
#include <QMessageBox>
#include <iostream>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    try {
        QApplication app(argc, argv);
        
        // Set application metadata
        QApplication::setApplicationName("Mexo Toolbox");
        QApplication::setApplicationVersion("1.0");
        QApplication::setOrganizationName("ImageProcessor");
        
        // Test basic Qt functionality first
        std::cout << "Qt Application created successfully" << std::endl;
        
        // Try to create a basic message box to test Qt
        QMessageBox testBox;
        testBox.setWindowTitle("Startup Test");
        testBox.setText("Qt is initializing...");
        testBox.setIcon(QMessageBox::Information);
        testBox.setStandardButtons(QMessageBox::Ok);
        
        // Show test message briefly
        testBox.show();
        app.processEvents();
        testBox.hide();
        
        std::cout << "Basic Qt widgets working" << std::endl;
        
        // Apply stylesheet
        QString styleSheet = QString(
            "QMainWindow {"
                "background-color: #0a0e27;"
            "}"
            
            "QWidget {"
                "background-color: #0f1535;"
                "color: #f8f9fc;"
                "font-family: 'Segoe UI', Arial;"
                "font-size: 10pt;"
            "}"
            
            "QPushButton {"
                "background-color: #1a2351;"
                "color: #f8f9fc;"
                "border: none;"
                "border-radius: 6px;"
                "padding: 12px;"
                "font-weight: normal;"
                "min-width: 120px;"
            "}"
            
            "QPushButton:hover {"
                "background-color: #252d48;"
                "color: #00d4ff;"
            "}"
            
            "QPushButton:pressed {"
                "background-color: #0099cc;"
            "}"
            
            "QPushButton[class=\"accent\"] {"
                "background-color: #00d4ff;"
                "color: #0a0e27;"
                "font-weight: bold;"
            "}"
            
            "QPushButton[class=\"accent\"]:hover {"
                "background-color: #00e8ff;"
            "}"
            
            "QLineEdit, QSpinBox, QDoubleSpinBox {"
                "background-color: #1a2351;"
                "color: #f8f9fc;"
                "border: 1px solid #3a4a6f;"
                "border-radius: 6px;"
                "padding: 8px;"
            "}"
            
            "QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus {"
                "border: 2px solid #00d4ff;"
            "}"
            
            "QSlider::groove:horizontal {"
                "background: #1a2351;"
                "height: 8px;"
                "border-radius: 4px;"
            "}"
            
            "QSlider::handle:horizontal {"
                "background: #00d4ff;"
                "width: 18px;"
                "margin: -5px 0;"
                "border-radius: 9px;"
            "}"
            
            "QProgressBar {"
                "background-color: #1a2351;"
                "border: none;"
                "border-radius: 4px;"
                "height: 8px;"
                "text-align: center;"
            "}"
            
            "QProgressBar::chunk {"
                "background-color: #00d4ff;"
                "border-radius: 4px;"
            "}"
            
            "QLabel {"
                "background-color: transparent;"
                "color: #f8f9fc;"
            "}"
            
            "QGroupBox {"
                "border: 1px solid #3a4a6f;"
                "border-radius: 8px;"
                "margin-top: 12px;"
                "padding: 15px;"
                "font-weight: bold;"
                "color: #00d4ff;"
            "}"
            
            "QGroupBox::title {"
                "subcontrol-origin: margin;"
                "subcontrol-position: top left;"
                "padding: 0 10px;"
            "}"
            
            "QScrollBar:vertical {"
                "background: #0f1535;"
                "width: 12px;"
                "border-radius: 6px;"
            "}"
            
            "QScrollBar::handle:vertical {"
                "background: #1a2351;"
                "border-radius: 6px;"
            "}"
            
            "QScrollBar::handle:vertical:hover {"
                "background: #00d4ff;"
            "}"
            
            "QStatusBar {"
                "background-color: #0f1535;"
                "color: #f8f9fc;"
            "}"
            
            "QMenuBar {"
                "background-color: #0f1535;"
                "color: #f8f9fc;"
                "border-bottom: 1px solid #3a4a6f;"
            "}"
            
            "QMenuBar::item {"
                "padding: 8px 12px;"
                "background: transparent;"
            "}"
            
            "QMenuBar::item:selected {"
                "background-color: #1a2351;"
                "color: #00d4ff;"
            "}"
            
            "QMenu {"
                "background-color: #1a2351;"
                "color: #f8f9fc;"
                "border: 1px solid #3a4a6f;"
            "}"
            
            "QMenu::item {"
                "padding: 8px 25px;"
            "}"
            
            "QMenu::item:selected {"
                "background-color: #252d48;"
                "color: #00d4ff;"
            "}"
            
            "QToolBar {"
                "background-color: #0f1535;"
                "border: none;"
                "spacing: 3px;"
            "}"
            
            "QTextEdit {"
                "background-color: #0f1535;"
                "color: #f8f9fc;"
                "border: 1px solid #3a4a6f;"
                "border-radius: 6px;"
                "padding: 10px;"
            "}"
            
            "QDialog {"
                "background-color: #0a0e27;"
            "}"
            
            "QMessageBox {"
                "background-color: #0f1535;"
                "color: #f8f9fc;"
            "}"
        );
        
        app.setStyleSheet(styleSheet);
        std::cout << "Stylesheet applied" << std::endl;
        
        // Try to create main window
        std::cout << "Creating main window..." << std::endl;
        MainWindow window;
        
        std::cout << "Main window created, showing..." << std::endl;
        window.show();
        
        std::cout << "Application ready, starting event loop" << std::endl;
        return app.exec();
        
    } catch (const std::exception& e) {
        std::cerr << "Exception caught in main: " << e.what() << std::endl;
        
        // Try to show error in message box if possible
        QApplication app(argc, argv);
        QMessageBox::critical(nullptr, "Application Error", 
                             QString("An error occurred during startup:\n\n%1")
                             .arg(e.what()));
        return -1;
    } catch (...) {
        std::cerr << "Unknown exception caught in main" << std::endl;
        return -1;
    }
}