@echo off
echo Building Console Version of Mexo Toolbox...
echo.

REM Create build directory for console version
if not exist "build_console" mkdir build_console
cd build_console

REM Set OpenCV path - adjust if different
set OpenCV_DIR=F:\OpenCV\opencv\build

echo Using OpenCV at: %OpenCV_DIR%
echo.

REM Copy the console CMakeLists to build directory with correct name
copy ..\CMakeLists_console.txt CMakeLists.txt

REM Build using CMake
cmake .
cmake --build . --config Release

echo.
if exist Release\console_app.exe (
    echo ? Build successful! Executable: build_console\Release\console_app.exe
    echo.
    echo To test, run:
    echo cd build_console\Release
    echo console_app.exe
    echo.
    echo Copy an image to that folder first for testing.
) else if exist console_app.exe (
    echo ? Build successful! Executable: build_console\console_app.exe
    echo.
    echo To test, run:
    echo cd build_console
    echo console_app.exe
    echo.
    echo Copy an image to that folder first for testing.
) else (
    echo ? Build failed! Check error messages above.
)

cd ..
pause