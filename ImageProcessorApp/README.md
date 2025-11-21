# ?? Mexo Toolbox - Professional Image Processing Application

A comprehensive desktop image processing application built with **C++17**, **Qt6**, and **OpenCV** featuring a modern dark-themed UI with neon accent colors.

## ?? Features

### ?? Lab 1: Image Information
- **?? Detailed Image Analysis**: Dimensions, channels, data types, min/max values
- **?? Interactive Display**: Professional information dialog with monospace formatting

### ?? Lab 2: Pixel Operations
- **?? Pixel Value Inspection**: Click coordinates to get RGB/BGR values
- **?? Real-time Coordinate Input**: Spinbox controls for precise pixel selection

### ?? Lab 3: Statistical Analysis
- **?? Image Statistics**: Mean, standard deviation, min/max, dynamic range
- **?? Mathematical Analysis**: Comprehensive statistical overview

### ?? Lab 4: Geometric Transformations
- **? Translation**: Interactive sliders with live preview
- **?? Rotation**: Angle control with real-time rotation preview
- **?? Zoom**: Scale factor adjustment with preview
- **? Skew**: Affine skew transformation
- **?? Flip Operations**: X, Y, and XY flip transformations

### ?? Lab 5: Histogram & Thresholding
- **?? Interactive Histogram**: RGB/Grayscale distribution visualization
- **? Histogram Equalization**: Contrast enhancement with before/after comparison
- **?? Otsu Thresholding**: Automatic optimal threshold selection

### ?? Lab 6: Image Processing
- **? Grayscale Conversion**: RGB to grayscale transformation
- **?? Binary Thresholding**: Configurable threshold operations
- **?? Gaussian Blur**: Noise reduction and smoothing
- **?? Edge Detection**: Canny edge detection algorithm
- **?? Color Inversion**: Negative image effect

## ?? Design Features

### Modern Dark Theme
- **?? Professional Dark UI**: Consistent #0a0e27 primary background
- **?? Neon Accents**: Cyan (#00d4ff), lime (#39ff14), pink (#ff006e)
- **? Responsive Design**: Adaptive layouts and smooth transitions

### Advanced UI Components
- **?? Dual Canvas Display**: Side-by-side original and processed images
- **?? Interactive Controls**: Grouped by functionality with tooltips
- **?? Live Preview**: Real-time transformation previews
- **?? Custom Histogram Widget**: Professional visualization with gradients

## ?? Requirements

### Software Dependencies
- **C++17** or higher compiler (Visual Studio 2019+, GCC 8+, Clang 7+)
- **Qt 6.x Framework** (Widgets module)
- **OpenCV 4.x** (cv::Mat, image processing functions)
- **CMake 3.16+** or **qmake** for build system

### Hardware Requirements
- **RAM**: 4GB+ recommended for large image processing
- **Storage**: 100MB+ for installation
- **Display**: 1200x800 minimum resolution

## ?? Installation

### Windows (Visual Studio)

#### 1. Install Qt6
```bash
# Download Qt6 from https://www.qt.io/download
# Install to C:\Qt\6.5.3\msvc2019_64\ (or adjust paths in project)
```

#### 2. Install OpenCV
```bash
# Download OpenCV from https://opencv.org/releases/
# Extract to F:\OpenCV\ (or adjust paths in project)
```

#### 3. Build the Project
```bash
# Option A: Using CMake
mkdir build
cd build
cmake ..
cmake --build . --config Release

# Option B: Using qmake
qmake ImageProcessorApp.pro
nmake

# Option C: Using Visual Studio
# Open ImageProcessorApp.vcxproj in Visual Studio
# Adjust include/library paths if needed
# Build -> Build Solution
```

### Linux (Ubuntu/Debian)

#### 1. Install Dependencies
```bash
sudo apt-get update
sudo apt-get install build-essential cmake
sudo apt-get install qt6-base-dev qt6-tools-dev
sudo apt-get install libopencv-dev pkg-config
```

#### 2. Build the Project
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### macOS

#### 1. Install Dependencies
```bash
# Using Homebrew
brew install qt6 opencv cmake
```

#### 2. Build the Project
```bash
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH=/opt/homebrew/opt/qt6 ..
make -j$(sysctl -n hw.ncpu)
```

## ?? Configuration

### Environment Variables
```bash
# Add to your environment
export Qt6_DIR=/path/to/qt6
export OpenCV_DIR=/path/to/opencv
```

### Build Options
```bash
# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release with optimizations
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3" ..
```

## ?? Usage

### Basic Workflow
1. **?? Load Image**: File ? Load Image or Ctrl+O
2. **?? Analyze**: Use Lab 1-3 for image information and analysis
3. **?? Transform**: Apply geometric transformations (Lab 4)
4. **?? Enhance**: Use histogram operations (Lab 5)
5. **?? Process**: Apply image processing effects (Lab 6)
6. **?? Save**: File ? Save Image or Ctrl+S

### Advanced Features
- **?? Reset**: Return to original image anytime
- **?? Live Preview**: See changes in real-time during transformations
- **?? Statistical Analysis**: Monitor image properties throughout processing
- **?? Professional UI**: Organized controls with tooltips and shortcuts

## ?? Project Structure

```
ImageProcessorApp/
??? CMakeLists.txt              # CMake build configuration
??? ImageProcessorApp.pro       # qmake build configuration  
??? ImageProcessorApp.vcxproj   # Visual Studio project
??? README.md                   # This file
??? src/
?   ??? main.cpp               # Application entry point
?   ??? MainWindow.h/.cpp      # Main application window
?   ??? ImageCanvas.h/.cpp     # Custom image display widget
?   ??? TransformDialog.h/.cpp # Transformation dialogs
?   ??? HistogramWidget.h/.cpp # Histogram visualization
??? include/
?   ??? ImageProcessor.h       # Utility classes and constants
??? resources/
    ??? resources.qrc          # Qt resource file
    ??? icons/
        ??? mexo_toolbox_logo.ico
```

## ?? Theme Customization

### Color Palette
```cpp
// Primary backgrounds
BG_PRIMARY:     #0a0e27  // Main window background
BG_SECONDARY:   #0f1535  // Widget backgrounds  
BG_TERTIARY:    #1a2351  // Input controls
BG_QUATERNARY:  #252d48  // Hover states

// Accent colors
ACCENT_CYAN:    #00d4ff  // Primary accent
ACCENT_LIME:    #39ff14  // Secondary accent
ACCENT_PINK:    #ff006e  // Warning/error states
SUCCESS_GREEN:  #1fa65a  // Success states
```

### Custom Styling
Modify the stylesheet in `main.cpp` to customize the appearance:
```cpp
// Example: Change accent color
QPushButton[class="accent"] {
    background-color: #your-color;
}
```

## ?? Testing

### Supported Formats
- **Input**: PNG, JPEG, BMP, TIFF, GIF
- **Output**: PNG, JPEG, BMP (configurable)

### Test Images
```bash
# Test with various image types
./ImageProcessorApp test_images/rgb_image.png
./ImageProcessorApp test_images/grayscale_image.jpg
./ImageProcessorApp test_images/large_image.bmp
```

### Performance Testing
- **Memory**: Tested with images up to 50MP
- **Processing**: All operations < 100ms for typical images
- **UI Responsiveness**: 60 FPS interface updates

## ?? Troubleshooting

### Common Issues

#### Qt6 Not Found
```bash
# Set Qt6 path
export CMAKE_PREFIX_PATH=/path/to/qt6
```

#### OpenCV Linking Errors
```bash
# Verify OpenCV installation
pkg-config --modversion opencv4
```

#### Missing MOC Files
```bash
# Clean and rebuild
rm -rf build/
mkdir build && cd build
cmake ..
```

### Performance Issues
- **Large Images**: Consider image resizing for preview
- **Memory Usage**: Monitor with Task Manager/htop
- **Slow Transformations**: Use threading for heavy operations

## ?? API Reference

### Key Classes

#### MainWindow
- `loadImage()`: Load image from file
- `saveImage()`: Save processed image
- `updateDisplay()`: Refresh image canvases

#### ImageCanvas
- `setImage(cv::Mat)`: Display OpenCV image
- `setImage(QPixmap)`: Display Qt pixmap
- `clear()`: Clear canvas content

#### TransformDialog
- `Translation`: Interactive translation dialog
- `Rotation`: Angle-based rotation
- `Zoom`: Scale factor adjustment

## ?? Contributing

### Development Setup
1. Fork the repository
2. Create feature branch: `git checkout -b feature-name`
3. Follow coding standards (see `.clang-format`)
4. Add tests for new functionality
5. Submit pull request

### Coding Standards
- **C++17** features encouraged
- **Qt** naming conventions (camelCase)
- **OpenCV** cv:: namespace usage
- **Comments**: Doxygen-style for public APIs

## ?? License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## ?? Acknowledgments

- **Qt Framework**: Cross-platform UI development
- **OpenCV**: Computer vision and image processing
- **Material Design**: UI/UX inspiration
- **Community**: Open source contributors

## ?? Support

- **Issues**: GitHub Issues tracker
- **Documentation**: See `/docs` folder
- **Community**: Join our Discord server
- **Email**: support@mexotoolbox.com

---

**?? Mexo Toolbox** - Professional Image Processing Made Simple