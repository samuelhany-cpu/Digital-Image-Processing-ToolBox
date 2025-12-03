# ?? Mexo Toolbox - Digital Image Processing Application

A professional desktop application for digital image processing built with Qt6 and OpenCV, featuring a modern dark-themed UI with comprehensive image manipulation capabilities.

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![Qt Version](https://img.shields.io/badge/Qt-6.7.3-41CD52)
![OpenCV](https://img.shields.io/badge/OpenCV-4.3.0-blue)
![C++](https://img.shields.io/badge/C%2B%2B-14-00599C)
![License](https://img.shields.io/badge/license-MIT-blue)

## ? Features

### ?? Lab 1-3: Image Analysis
- **Image Information** - Detailed file properties, dimensions, data types
- **Pixel Operations** - Interactive pixel value inspection with coordinate input
- **Statistical Analysis** - Mean, standard deviation, min/max values, dynamic range

### ?? Lab 4: Geometric Transformations
- **Translation** - Move images with live preview sliders
- **Rotation** - Angle-based rotation with real-time feedback
- **Zoom** - Scale factor adjustment with preview
- **Skew** - Affine transformation capabilities
- **Flip** - Horizontal, vertical, and both-axis flipping

### ?? Lab 5: Histogram & Thresholding
- **Histogram Visualization** - RGB/Grayscale distribution analysis
- **Histogram Equalization** - Contrast enhancement with before/after comparison
- **Otsu Thresholding** - Automatic binary thresholding

### ?? Lab 6: Image Processing
- **Grayscale Conversion** - Color to grayscale transformation
- **Binary Thresholding** - Configurable threshold values
- **Gaussian Blur** - Noise reduction filtering
- **Canny Edge Detection** - Advanced edge detection algorithm
- **Color Inversion** - Negative image creation

## ??? User Interface

- **Modern Dark Theme** - Professional #0a0e27 background with neon accents
- **Dual Canvas Display** - Side-by-side original and processed image comparison
- **Live Previews** - Real-time transformation feedback
- **Organized Controls** - Grouped by lab functionality with tooltips
- **Responsive Design** - Resizable panels and intuitive layout

## ??? Technical Stack

- **Language:** C++14
- **GUI Framework:** Qt 6.7.3
- **Image Processing:** OpenCV 4.3.0
- **Build System:** CMake + Visual Studio 2022
- **Architecture:** 64-bit Windows

## ?? Getting Started

### Prerequisites
- Qt 6.7.3 (MSVC 2019 64-bit)
- OpenCV 4.3.0
- Visual Studio 2022 or CMake 3.16+
- Windows 10/11 64-bit

### Installation

1. **Clone the repository**
```bash
git clone https://github.com/samuelhany-cpu/Digital-Image-Processing-ToolBox.git
cd Digital-Image-Processing-ToolBox
```

2. **Install dependencies**
   - Download and install Qt 6.7.3 from [qt.io](https://www.qt.io/download-open-source)
   - Download OpenCV 4.3.0 from [opencv.org](https://opencv.org/releases/)

3. **Build the project**
```bash
cd ImageProcessorApp
.\build_custom.bat
```

### Running the Application

```bash
cd build\Release
ImageProcessorApp.exe
```

## ?? Usage

1. **Load an Image** - Click "Load Image" or File ? Load Image
2. **Select Processing** - Choose from Labs 1-6 in the control panel
3. **View Results** - See original and processed images side-by-side
4. **Save Results** - Export processed images in PNG, JPEG, or BMP format

## ?? Project Structure

```
Digital-Image-Processing-ToolBox/
??? ImageProcessorApp/              # Main Qt GUI application
?   ??? src/                        # Source files
?   ?   ??? main.cpp
?   ?   ??? MainWindow.cpp
?   ?   ??? ImageCanvas.cpp
?   ?   ??? TransformDialog.cpp
?   ?   ??? HistogramWidget.cpp
?   ??? include/                    # Header files
?   ?   ??? ImageProcessor.h
?   ??? resources/                  # Icons and resources
?   ??? CMakeLists.txt             # CMake configuration
?   ??? README.md                  # Documentation
?
??? Project1/                       # Original lab projects
?   ??? Lab1.cpp - Lab6.cpp        # Individual lab implementations
?   ??? translation.jpg            # Test image
?
??? README.md                       # This file
```

## ?? Key Components

### MainWindow
Main application window with menu bar, toolbar, dual canvas display, and processing controls.

### ImageCanvas
Custom widget for displaying images with zoom and pan capabilities.

### TransformDialog
Interactive dialog for geometric transformations with live preview.

### HistogramWidget
Custom widget for visualizing image histograms with RGB/grayscale analysis.

### ImageProcessor
Header-only library with core image processing algorithms.

## ?? Testing

Test images are provided in the `Project1` directory. The application has been tested with:
- ? PNG, JPEG, BMP, TIFF formats
- ? Grayscale and color images
- ? Various resolutions (tested up to 4K)

## ?? Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## ?? License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## ????? Author

**Samuel Hany**
- GitHub: [@samuelhany-cpu](https://github.com/samuelhany-cpu)

## ?? Acknowledgments

- Qt Framework for the excellent GUI toolkit
- OpenCV community for the powerful image processing library
- Visual Studio team for the development environment

## ?? Screenshots

*Coming soon - Add screenshots of your application in action!*

---

**? Star this repository if you find it helpful!**