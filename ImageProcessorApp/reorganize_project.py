# Professional Project Reorganization Script

import os
import shutil

def create_directory_structure():
    """Create a professional directory structure"""
    
    directories = [
        # Source code organization
        'src/core',           # Core functionality
        'src/ui',             # UI components
        'src/dialogs',        # Dialog windows
        'src/widgets',        # Custom widgets
        'src/utils',          # Utility functions
        'src/filters',        # Filter implementations
        'src/processing',     # Image processing algorithms
        
        # Resources
        'resources/icons',    # Application icons
        'resources/images',   # Sample images
        'resources/styles',   # Style sheets
        
        # Documentation
        'docs',               # Documentation files
        'docs/api',           # API documentation
        'docs/guides',        # User guides
        
        # Build and config
        'build',              # Build output
        'config',             # Configuration files
        
        # Tests
        'tests',              # Unit tests
        'tests/data',         # Test data
    ]
    
    for directory in directories:
        os.makedirs(directory, exist_ok=True)
        print(f"✓ Created: {directory}/")
    
    return directories

def reorganize_files():
    """Move files to their appropriate locations"""
    
    moves = [
        # UI Components
        ('src/MainWindow.cpp', 'src/ui/MainWindow.cpp'),
        ('src/MainWindow.h', 'src/ui/MainWindow.h'),
        
        # Dialogs
        ('src/TransformDialog.cpp', 'src/dialogs/TransformDialog.cpp'),
        ('src/TransformDialog.h', 'src/dialogs/TransformDialog.h'),
        
        # Widgets
        ('src/ImageCanvas.cpp', 'src/widgets/ImageCanvas.cpp'),
        ('src/ImageCanvas.h', 'src/widgets/ImageCanvas.h'),
        ('src/HistogramWidget.cpp', 'src/widgets/HistogramWidget.cpp'),
        ('src/HistogramWidget.h', 'src/widgets/HistogramWidget.h'),
        
        # Core/Utils
        ('include/ImageProcessor.h', 'src/core/ImageProcessor.h'),
        
        # Main entry point stays in src root
        # ('src/main.cpp', 'src/main.cpp'),  # Keep in src/
        
        # Documentation
        ('README.md', 'docs/README.md'),
        ('INSTALL.md', 'docs/INSTALL.md'),
        ('INSTALLATION_COMPLETE.md', 'docs/INSTALLATION_COMPLETE.md'),
        ('PROJECT_COMPLETE.md', 'docs/PROJECT_COMPLETE.md'),
        ('DIAGNOSIS_COMPLETE.md', 'docs/DIAGNOSIS_COMPLETE.md'),
        ('DLL_FIX_COMPLETE.md', 'docs/DLL_FIX_COMPLETE.md'),
        ('FILTERS_DOCUMENTATION.md', 'docs/FILTERS_DOCUMENTATION.md'),
        ('FILTERS_COMPLETE.md', 'docs/FILTERS_COMPLETE.md'),
        ('EMOJI_REMOVAL_COMPLETE.md', 'docs/EMOJI_REMOVAL_COMPLETE.md'),
        ('QUICK_START.md', 'docs/QUICK_START.md'),
    ]
    
    for src, dest in moves:
        if os.path.exists(src):
            os.makedirs(os.path.dirname(dest), exist_ok=True)
            shutil.copy2(src, dest)
            print(f"✓ Moved: {src} -> {dest}")
        else:
            print(f"⚠ Not found: {src}")

def create_professional_readme():
    """Create a professional README for the reorganized structure"""
    
    readme_content = '''# Mexo Toolbox - Professional Image Processing Application

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
.\\build_custom.bat

# Run the application
cd build\\Release
.\\ImageProcessorApp.exe
```

## Features

- **Lab 1-3**: Image Information & Analysis
- **Lab 4**: Geometric Transformations
- **Lab 5**: Histogram & Thresholding
- **Lab 6**: Image Processing
- **Lab 7**: Custom Filters (6 filters including Laplacian and Sobel)

## Documentation

See `docs/` folder for comprehensive documentation.
'''
    
    with open('PROJECT_STRUCTURE.md', 'w', encoding='utf-8') as f:
        f.write(readme_content)
    
    print("Created: PROJECT_STRUCTURE.md")

if __name__ == '__main__':
    print("=== Project Reorganization ===\n")
    print("Creating directory structure...")
    create_directory_structure()
    print("\nReorganizing files...")
    reorganize_files()
    print("\nCreating documentation...")
    create_professional_readme()
    print("\n✓ Project reorganization complete!")
    print("\nNote: Original files kept in place. New structure created alongside.")
