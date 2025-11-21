@echo off
echo ?? Building Simplified Main Window Test...
echo ========================================

if not exist "build_simplified" mkdir build_simplified
cd build_simplified

copy ..\CMakeLists_simplified.txt CMakeLists.txt

echo Configuring simplified main window...
set CMAKE_PREFIX_PATH=C:\Qt\6.7.3\msvc2019_64
cmake .

echo Building simplified main...
cmake --build . --config Release

cd Release

if exist simplified_main.exe (
    echo ? Simplified main built successfully!
    echo.
    
    copy "..\..\..\Project1\translation.jpg" . 2>nul
    
    echo ?? Running simplified main window...
    echo Console output:
    simplified_main.exe
    
    echo.
    echo If this works, the issue is with our complex MainWindow class.
) else (
    echo ? Simplified main build failed!
)

cd ..\..
pause