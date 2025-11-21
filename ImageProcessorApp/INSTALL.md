# ImageProcessorApp Installation Guide

## ?? Quick Setup for Your System

Based on your current setup, here's what you need to do:

### 1. ? Qt6 Status - READY!
Your system has Qt 6.7.3 installed in the C partition. Let me detect the exact path:

Common Qt 6.7.3 locations:
- `C:\Qt\6.7.3\msvc2019_64\`
- `C:\Qt\6.7.3\msvc2022_64\` 
- `C:\Qt\Tools\Qt6.7.3\msvc2019_64\`

### 2. Build Instructions for Your System

#### Using CMake (Recommended)
```bash
cd ImageProcessorApp
mkdir build
cd build

# Set Qt6 path (adjust based on your exact installation)
set CMAKE_PREFIX_PATH=C:\Qt\6.7.3\msvc2019_64

cmake ..
cmake --build . --config Release
```

#### Using qmake
```bash
cd ImageProcessorApp

# Set Qt6 in PATH (adjust based on your exact installation)  
set PATH=C:\Qt\6.7.3\msvc2019_64\bin;%PATH%

qmake ImageProcessorApp.pro
nmake
```

#### Using Visual Studio
1. ? Qt6 is already installed
2. Open `ImageProcessorApp.vcxproj` in Visual Studio
3. Update include paths in Project Properties if needed:
   - VC++ Directories ? Include Directories: `C:\Qt\6.7.3\msvc2019_64\include`
   - VC++ Directories ? Library Directories: `C:\Qt\6.7.3\msvc2019_64\lib`
4. Build ? Build Solution

### 3. Your Current Setup Status
? **OpenCV**: Already configured at `F:\OpenCV\opencv\build`  
? **Qt6**: Version 6.7.3 installed in C partition  
? **Visual Studio**: Not found (optional, CMake can work without it)

### 4. Quick Start
Run the setup script to detect your exact Qt path:
```bash
powershell -ExecutionPolicy Bypass -File setup.ps1
```

### 5. Alternative: Console Version First

If you want to test the image processing functionality immediately:
```bash
# Build and run console version (no Qt required)
build_console.bat

# Then run
cd build_console\Release
console_app.exe
```

### 6. Next Steps

I'll now:
1. Update the setup script to detect Qt 6.7.3
2. Update project files for C++14 compatibility 
3. Create a one-click build script for your system

Let me know if you'd like to proceed with the GUI version or try the console version first!