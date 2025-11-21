@echo off
echo ?? Building Simple Test Version...
echo ================================

REM Create test build directory
if not exist "build_test" mkdir build_test
cd build_test

REM Copy CMakeLists.txt
copy ..\CMakeLists_simple.txt CMakeLists.txt

echo Configuring simple test...
set CMAKE_PREFIX_PATH=C:\Qt\6.7.3\msvc2019_64
cmake .

echo Building simple test...
cmake --build . --config Release

cd Release

echo.
if exist simple_test.exe (
    echo ? Simple test built successfully!
    echo.
    echo ?? Running simple test...
    simple_test.exe
) else (
    echo ? Simple test build failed!
)

cd ..\..
pause