QT += core widgets

CONFIG += c++14

TARGET = ImageProcessorApp
TEMPLATE = app

# Source files
SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp \
    src/ImageCanvas.cpp \
    src/TransformDialog.cpp \
    src/HistogramWidget.cpp

# Header files
HEADERS += \
    src/MainWindow.h \
    src/ImageCanvas.h \
    src/TransformDialog.h \
    src/HistogramWidget.h \
    include/ImageProcessor.h

# Resources
RESOURCES += resources/resources.qrc

# Include directories
INCLUDEPATH += include

# OpenCV configuration
win32 {
    # Adjust these paths according to your OpenCV installation
    INCLUDEPATH += "F:/OpenCV/opencv/build/include"
    
    CONFIG(debug, debug|release) {
        LIBS += -L"F:/OpenCV/opencv/build/x64/vc15/lib" \
                -lopencv_world430d
    }
    CONFIG(release, debug|release) {
        LIBS += -L"F:/OpenCV/opencv/build/x64/vc15/lib" \
                -lopencv_world430
    }
}

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv4
}

# Additional compiler flags for C++14
QMAKE_CXXFLAGS += -std=c++14

# Enable optimization in release mode
CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
    QMAKE_CXXFLAGS_RELEASE += -O3
}

# Windows specific settings
win32 {
    CONFIG += windows
    RC_ICONS = resources/icons/mexo_toolbox_logo.ico
}