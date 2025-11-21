#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Create a simple test window
    QWidget window;
    window.setWindowTitle("Qt Test Application");
    window.resize(400, 300);
    
    QVBoxLayout *layout = new QVBoxLayout(&window);
    
    QLabel *label = new QLabel("Qt6 is working!\nThis is a test window.");
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-size: 16pt; color: blue;");
    
    layout->addWidget(label);
    
    window.show();
    
    QMessageBox::information(&window, "Success", "Qt6 application started successfully!");
    
    return app.exec();
}