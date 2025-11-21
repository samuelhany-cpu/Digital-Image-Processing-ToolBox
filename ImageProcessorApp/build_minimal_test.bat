@echo off
echo ?? Building Minimal OpenCV Test...
echo ===============================

REM Create minimal build directory
if not exist "build_minimal" mkdir build_minimal
cd build_minimal

REM Copy CMakeLists.txt
copy ..\CMakeLists_minimal.txt CMakeLists.txt

echo Configuring minimal OpenCV test...
set CMAKE_PREFIX_PATH=C:\Qt\6.7.3\msvc2019_64
set OpenCV_DIR=F:\OpenCV\opencv\build
cmake .

echo Building minimal test...
cmake --build . --config Release

cd Release

echo.
if exist minimal_test.exe (
    echo ? Minimal OpenCV test built successfully!
    echo.
    
    REM Copy test image
    copy "..\..\..\Project1\translation.jpg" . 2>nul
    
    echo ?? Running minimal OpenCV test...
    echo.
    echo Console output:
    minimal_test.exe
) else (
    echo ? Minimal test build failed!
)

cd ..\..
pause