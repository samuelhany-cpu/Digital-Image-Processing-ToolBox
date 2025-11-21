@echo off
echo ?? Mexo Toolbox - One-Click Build Script
echo ========================================
echo.

REM Detect Qt 6.7.3 installation
set QT6_PATH=
if exist "C:\Qt\6.7.3\msvc2019_64" (
    set QT6_PATH=C:\Qt\6.7.3\msvc2019_64
    echo ? Found Qt 6.7.3 at: %QT6_PATH%
) else if exist "C:\Qt\6.7.3\msvc2022_64" (
    set QT6_PATH=C:\Qt\6.7.3\msvc2022_64
    echo ? Found Qt 6.7.3 at: %QT6_PATH%
) else if exist "C:\Qt\Tools\Qt6.7.3\msvc2019_64" (
    set QT6_PATH=C:\Qt\Tools\Qt6.7.3\msvc2019_64
    echo ? Found Qt 6.7.3 at: %QT6_PATH%
) else (
    echo ? Qt 6.7.3 not found in standard locations!
    echo Please install Qt 6.7.3 or update the paths in this script.
    pause
    exit /b 1
)

REM Detect OpenCV installation
set OPENCV_PATH=
if exist "F:\OpenCV\opencv\build" (
    set OPENCV_PATH=F:\OpenCV\opencv\build
    echo ? Found OpenCV at: %OPENCV_PATH%
) else if exist "C:\opencv\build" (
    set OPENCV_PATH=C:\opencv\build
    echo ? Found OpenCV at: %OPENCV_PATH%
) else (
    echo ? OpenCV not found in standard locations!
    echo Please install OpenCV or update the paths in this script.
    pause
    exit /b 1
)

echo.
echo ?? Build Options:
echo 1. GUI Version (Full Qt Application)
echo 2. Console Version (OpenCV only)
echo 3. Both versions
echo.
set /p choice="Choose option (1-3): "

if "%choice%"=="1" goto build_gui
if "%choice%"=="2" goto build_console
if "%choice%"=="3" goto build_both
echo Invalid choice!
pause
exit /b 1

:build_gui
echo.
echo ??? Building GUI Version...
echo ========================

REM Set environment
set PATH=%QT6_PATH%\bin;%PATH%
set CMAKE_PREFIX_PATH=%QT6_PATH%
set OpenCV_DIR=%OPENCV_PATH%

REM Create build directory
if not exist build mkdir build
cd build

REM Configure with CMake
echo Configuring with CMake...
cmake -DCMAKE_PREFIX_PATH=%QT6_PATH% -DOpenCV_DIR=%OPENCV_PATH% ..

if errorlevel 1 (
    echo ? CMake configuration failed!
    pause
    exit /b 1
)

REM Build
echo Building application...
cmake --build . --config Release

if exist Release\ImageProcessorApp.exe (
    echo.
    echo ? GUI Build successful!
    echo Executable: build\Release\ImageProcessorApp.exe
    echo.
    echo To run: cd build\Release && ImageProcessorApp.exe
) else (
    echo ? GUI Build failed!
)

cd ..
goto end

:build_console
echo.
echo ??? Building Console Version...
echo ============================

REM Set environment
set OpenCV_DIR=%OPENCV_PATH%

REM Create build directory
if not exist build_console mkdir build_console
cd build_console

REM Configure with CMake
echo Configuring console version with CMake...
cmake -DOpenCV_DIR=%OPENCV_PATH% -f ..\CMakeLists_console.txt ..

if errorlevel 1 (
    echo ? Console CMake configuration failed!
    pause
    exit /b 1
)

REM Build
echo Building console application...
cmake --build . --config Release

if exist Release\console_app.exe (
    echo.
    echo ? Console Build successful!
    echo Executable: build_console\Release\console_app.exe
    echo.
    echo To run: cd build_console\Release && console_app.exe
) else (
    echo ? Console Build failed!
)

cd ..
goto end

:build_both
call :build_console
call :build_gui
goto end

:end
echo.
echo ?? Build process completed!
echo.
echo ?? Project structure:
echo    - GUI version: build\Release\ImageProcessorApp.exe
echo    - Console version: build_console\Release\console_app.exe
echo    - Test with sample images from your existing Project1 folder
echo.
pause