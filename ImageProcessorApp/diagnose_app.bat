@echo off
echo ?? ImageProcessorApp Comprehensive Diagnostics
echo =============================================
echo.

cd build\Release

echo ?? Current Directory: %CD%
echo.

echo ?? Files in current directory:
dir /b
echo.

echo ?? Checking for missing MSVC Runtime DLLs...
echo.

REM Check for MSVC Runtime DLLs
set MSVC_RUNTIME_MISSING=0

if not exist "C:\Windows\System32\vcruntime140.dll" (
    echo ? vcruntime140.dll missing from System32
    set MSVC_RUNTIME_MISSING=1
) else (
    echo ? vcruntime140.dll found in System32
)

if not exist "C:\Windows\System32\msvcp140.dll" (
    echo ? msvcp140.dll missing from System32  
    set MSVC_RUNTIME_MISSING=1
) else (
    echo ? msvcp140.dll found in System32
)

if not exist "C:\Windows\System32\vcruntime140_1.dll" (
    echo ?? vcruntime140_1.dll missing from System32 (may be needed)
) else (
    echo ? vcruntime140_1.dll found in System32
)

echo.
echo ?? Qt6 DLL Verification:
for %%f in (Qt6Core.dll Qt6Gui.dll Qt6Widgets.dll Qt6Network.dll Qt6OpenGL.dll) do (
    if exist "%%f" (
        echo ? %%f present
    ) else (
        echo ? %%f MISSING
    )
)

echo.
echo ?? Platform Plugins Check:
if exist "platforms\qwindows.dll" (
    echo ? platforms\qwindows.dll present
) else (
    echo ? platforms\qwindows.dll MISSING
)

if exist "platforms\qminimal.dll" (
    echo ? platforms\qminimal.dll present  
) else (
    echo ? platforms\qminimal.dll MISSING
)

echo.
echo ?? OpenCV DLL Check:
if exist "opencv_world430.dll" (
    echo ? opencv_world430.dll present
) else (
    echo ? opencv_world430.dll MISSING
)

echo.
echo ?? Testing Application Startup...
echo.
echo Method 1: Direct execution with error capture
ImageProcessorApp.exe 2>&1
set APP_RESULT=%ERRORLEVEL%

echo.
echo Exit code: %APP_RESULT%

if %APP_RESULT% EQU 0 (
    echo ? Application started successfully
) else (
    echo ? Application failed with exit code: %APP_RESULT%
)

echo.
echo ?? Recommendations:
echo.
if %MSVC_RUNTIME_MISSING% EQU 1 (
    echo 1. Install Microsoft Visual C++ Redistributable 2019/2022
    echo    Download from: https://docs.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist
    echo.
)

echo 2. Try running as Administrator (right-click ? Run as administrator)
echo 3. Check Windows Event Viewer for detailed error information:
echo    Windows Logs ? Application ? Look for ImageProcessorApp errors
echo.
echo 4. Try the console version to verify OpenCV functionality:
echo    cd ..\..\build_console\Release
echo    console_app.exe
echo.

if %APP_RESULT% NEQ 0 (
    echo ?? If application still doesn't start, copy the exact error message
    echo    and we can create a simpler version or debug further.
)

cd ..\..
pause