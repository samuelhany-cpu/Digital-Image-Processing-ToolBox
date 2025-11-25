# Mexo Toolbox - Professional Image Processing Application

## Project Structure

```
ImageProcessorApp/
├── src/                          # Source code
│   ├── ui/                       # User Interface
│   │   ├── MainWindow.cpp/h      # Main application window
│   ├── dialogs/                  # Dialog windows
│   │   └── TransformDialog.cpp/h # Transformation dialogs
│   ├── widgets/                  # Custom widgets
│   │   ├── ImageCanvas.cpp/h     # Image display widget
│   │   └── HistogramWidget.cpp/h # Histogram visualization
│   ├── core/                     # Core functionality
│   │   └── ImageProcessor.h      # Image processing utilities
│   ├── filters/                  # Filter implementations
│   ├── processing/               # Image processing algorithms
│   ├── utils/                    # Utility functions
│   └── main.cpp                  # Application entry point
│
├── resources/                    # Application resources
│   ├── icons/                    # Application icons
│   ├── images/                   # Sample images
│   └── styles/                   # Style sheets
│
├── docs/                         # Documentation
│   ├── guides/                   # User guides
│   └── api/                      # API documentation
│
├── build/                        # Build output directory
├── config/                       # Configuration files
└── tests/                        # Unit tests

```

## Build Instructions

```bash
# Build the application
.\build_custom.bat

# Run the application
cd build\Release
.\ImageProcessorApp.exe
```

## Features

- **Lab 1-3**: Image Information & Analysis
- **Lab 4**: Geometric Transformations
- **Lab 5**: Histogram & Thresholding
- **Lab 6**: Image Processing
- **Lab 7**: Custom Filters (6 filters including Laplacian and Sobel)

## Documentation

See `docs/` folder for comprehensive documentation.
