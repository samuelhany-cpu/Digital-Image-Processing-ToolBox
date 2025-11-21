@echo off
echo Building Mexo Toolbox with detected paths...
echo.

REM Set detected paths
set QT6_PATH=C:\Qt\6.7.3\msvc2019_64
set OPENCV_PATH=F:\OpenCV\opencv\build

echo Using Qt6 at: %QT6_PATH%
echo Using OpenCV at: %OPENCV_PATH%
echo.

REM Create build directory
if not exist build mkdir build
cd build

REM Configure with CMake
set CMAKE_PREFIX_PATH=%QT6_PATH%
set OpenCV_DIR=%OPENCV_PATH%

cmake -DCMAKE_PREFIX_PATH=%QT6_PATH% -DOpenCV_DIR=%OPENCV_PATH% ..

REM Build
cmake --build . --config Release

echo.
if exist Release\ImageProcessorApp.exe (
    echo ? Build successful! 
    echo Executable: build\Release\ImageProcessorApp.exe
    echo.
    echo Run the application:
    echo cd Release
    echo ImageProcessorApp.exe
) else (
    echo ? Build failed! Check error messages above.
)

pause
