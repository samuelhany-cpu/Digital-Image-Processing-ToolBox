@echo off
echo Setting up Qt 6.7.3 MOC compilation...

REM Detect Qt 6.7.3 path
set QT6_DIR=
if exist "C:\Qt\6.7.3\msvc2019_64" (
    set QT6_DIR=C:\Qt\6.7.3\msvc2019_64
) else if exist "C:\Qt\6.7.3\msvc2022_64" (
    set QT6_DIR=C:\Qt\6.7.3\msvc2022_64
) else if exist "C:\Qt\Tools\Qt6.7.3\msvc2019_64" (
    set QT6_DIR=C:\Qt\Tools\Qt6.7.3\msvc2019_64
) else (
    echo Qt 6.7.3 not found! Please update the path in this script.
    pause
    exit /b 1
)

echo Using Qt 6.7.3 at: %QT6_DIR%
set PATH=%QT6_DIR%\bin;%PATH%

REM Create moc files
echo Generating MOC files...
moc src/MainWindow.h -o moc_MainWindow.cpp
moc src/ImageCanvas.h -o moc_ImageCanvas.cpp  
moc src/TransformDialog.h -o moc_TransformDialog.cpp
moc src/HistogramWidget.h -o moc_HistogramWidget.cpp

REM Create resource file
echo Generating resource file...
rcc resources/resources.qrc -o qrc_resources.cpp

echo MOC compilation complete!
echo.
echo Build options:
echo 1. CMake: run build_all.bat
echo 2. qmake: qmake ImageProcessorApp.pro && nmake
echo 3. Visual Studio: Open ImageProcessorApp.vcxproj

pause