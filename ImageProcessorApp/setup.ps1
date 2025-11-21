# PowerShell setup script for ImageProcessorApp
Write-Host "?? Mexo Toolbox Setup Script" -ForegroundColor Cyan
Write-Host "===============================" -ForegroundColor Cyan

# Check for Qt6.7.3
Write-Host "?? Checking for Qt 6.7.3..." -ForegroundColor Yellow
$Qt6Paths = @(
    "C:\Qt\6.7.3\msvc2019_64",
    "C:\Qt\6.7.3\msvc2022_64",
    "C:\Qt\Tools\Qt6.7.3\msvc2019_64",
    "C:\Qt\Tools\Qt6.7.3\msvc2022_64",
    "C:\Qt6.7.3\msvc2019_64",
    "C:\Qt6.7.3\msvc2022_64"
)

$Qt6Found = $false
$Qt6Path = ""
foreach ($path in $Qt6Paths) {
    if (Test-Path $path) {
        Write-Host "? Found Qt 6.7.3 at: $path" -ForegroundColor Green
        $env:Qt6_DIR = $path
        $env:PATH += ";$path\bin"
        $Qt6Found = $true
        $Qt6Path = $path
        break
    }
}

if (-not $Qt6Found) {
    Write-Host "? Qt 6.7.3 not found in expected locations!" -ForegroundColor Red
    Write-Host "   Please check if Qt 6.7.3 is installed in:" -ForegroundColor Yellow
    foreach ($path in $Qt6Paths) {
        Write-Host "   - $path" -ForegroundColor Gray
    }
    Write-Host "   Or specify the correct path manually." -ForegroundColor Yellow
}

# Check for OpenCV
Write-Host "?? Checking for OpenCV..." -ForegroundColor Yellow
$OpenCVPaths = @(
    "F:\OpenCV\opencv\build",
    "C:\opencv\build",
    "C:\OpenCV\build"
)

$OpenCVFound = $false
$OpenCVPath = ""
foreach ($path in $OpenCVPaths) {
    if (Test-Path $path) {
        Write-Host "? Found OpenCV at: $path" -ForegroundColor Green
        $env:OpenCV_DIR = $path
        $OpenCVFound = $true
        $OpenCVPath = $path
        break
    }
}

if (-not $OpenCVFound) {
    Write-Host "? OpenCV not found! Please install OpenCV from https://opencv.org/releases/" -ForegroundColor Red
}

# Check for Visual Studio
Write-Host "?? Checking for Visual Studio..." -ForegroundColor Yellow
$VSPaths = @(
    "${env:ProgramFiles(x86)}\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe",
    "${env:ProgramFiles(x86)}\Microsoft Visual Studio\2019\Professional\MSBuild\Current\Bin\MSBuild.exe",
    "${env:ProgramFiles}\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe",
    "${env:ProgramFiles}\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe"
)

$VSFound = $false
foreach ($path in $VSPaths) {
    if (Test-Path $path) {
        Write-Host "? Found Visual Studio MSBuild at: $path" -ForegroundColor Green
        $VSFound = $true
        break
    }
}

if (-not $VSFound) {
    Write-Host "?? Visual Studio not found, but CMake can still be used" -ForegroundColor Yellow
}

# Summary
Write-Host "`n?? Setup Summary:" -ForegroundColor Cyan
Write-Host "=================" -ForegroundColor Cyan
if ($Qt6Found) { 
    Write-Host "? Qt 6.7.3: Ready at $Qt6Path" -ForegroundColor Green 
} else { 
    Write-Host "? Qt 6.7.3: Missing" -ForegroundColor Red 
}
if ($OpenCVFound) { 
    Write-Host "? OpenCV: Ready at $OpenCVPath" -ForegroundColor Green 
} else { 
    Write-Host "? OpenCV: Missing" -ForegroundColor Red 
}
if ($VSFound) { 
    Write-Host "? Visual Studio: Ready" -ForegroundColor Green 
} else { 
    Write-Host "?? Visual Studio: Not found (optional)" -ForegroundColor Yellow 
}

if ($Qt6Found -and $OpenCVFound) {
    Write-Host "`n?? Ready to build! Generating build script..." -ForegroundColor Green
    
    # Create custom build script with detected paths
    $buildScript = @"
@echo off
echo Building Mexo Toolbox with detected paths...
echo.

REM Set detected paths
set QT6_PATH=$Qt6Path
set OPENCV_PATH=$OpenCVPath

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
"@
    $buildScript | Out-File -FilePath "build_custom.bat" -Encoding ASCII
    Write-Host "? Created custom build script: build_custom.bat" -ForegroundColor Green
    
    Write-Host "`n?? Build options:" -ForegroundColor Cyan
    Write-Host "1. CMake: run build_custom.bat" -ForegroundColor White
    Write-Host "2. qmake: qmake ImageProcessorApp.pro && nmake" -ForegroundColor White
    Write-Host "3. Visual Studio: Open ImageProcessorApp.vcxproj" -ForegroundColor White
    Write-Host "4. Console version: build_console.bat" -ForegroundColor White
} else {
    Write-Host "`n?? Missing dependencies. Please install the missing components." -ForegroundColor Yellow
}

Write-Host "`n?? For detailed instructions, see INSTALL.md" -ForegroundColor Cyan