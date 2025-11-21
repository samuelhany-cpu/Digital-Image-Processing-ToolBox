@echo off
echo ?? Qt6 Environment Setup and Application Test
echo ===========================================
echo.

REM Set Qt6 environment variables
set QT_DIR=C:\Qt\6.7.3\msvc2019_64
set PATH=%QT_DIR%\bin;%PATH%
set QT_PLUGIN_PATH=%QT_DIR%\plugins
set QT_QPA_PLATFORM_PLUGIN_PATH=%QT_DIR%\plugins\platforms

echo ?? Environment Variables:
echo QT_DIR: %QT_DIR%
echo QT_PLUGIN_PATH: %QT_PLUGIN_PATH%
echo QT_QPA_PLATFORM_PLUGIN_PATH: %QT_QPA_PLATFORM_PLUGIN_PATH%
echo.

echo ?? Current Directory: %CD%
echo.

echo ?? Verifying DLL files...
if exist "ImageProcessorApp.exe" (echo ? ImageProcessorApp.exe) else (echo ? ImageProcessorApp.exe MISSING & goto error)
if exist "Qt6Core.dll" (echo ? Qt6Core.dll) else (echo ? Qt6Core.dll MISSING & goto error)
if exist "Qt6Gui.dll" (echo ? Qt6Gui.dll) else (echo ? Qt6Gui.dll MISSING & goto error)
if exist "Qt6Widgets.dll" (echo ? Qt6Widgets.dll) else (echo ? Qt6Widgets.dll MISSING & goto error)
if exist "platforms\qwindows.dll" (echo ? platforms\qwindows.dll) else (echo ? platforms\qwindows.dll MISSING & goto error)
if exist "opencv_world430.dll" (echo ? opencv_world430.dll) else (echo ? opencv_world430.dll MISSING & goto error)

echo.
echo ?? Attempting to start application...
echo.

REM Try method 1: Direct execution with verbose output
echo Method 1: Direct execution
set QT_LOGGING_RULES=*=true
ImageProcessorApp.exe
set APP_EXIT_CODE=%ERRORLEVEL%

echo.
echo Application exit code: %APP_EXIT_CODE%

if %APP_EXIT_CODE% EQU 0 (
    echo ? Application exited successfully
) else (
    echo ? Application failed with exit code %APP_EXIT_CODE%
)

echo.
echo ?? Additional Diagnostics:
echo.

REM Check for common Qt issues
if not exist "platforms\qwindows.dll" (
    echo ? Platform plugin missing - this is likely the issue!
    echo Please copy qwindows.dll to platforms\ directory
)

REM Try method 2: With explicit platform plugin
echo.
echo Method 2: Testing with explicit platform plugin
set QT_QPA_PLATFORM=windows
ImageProcessorApp.exe
set APP_EXIT_CODE2=%ERRORLEVEL%
echo Platform test exit code: %APP_EXIT_CODE2%

echo.
echo ?? Summary:
echo - Direct execution exit code: %APP_EXIT_CODE%
echo - Platform test exit code: %APP_EXIT_CODE2%
echo.
echo If both methods failed, the issue might be:
echo 1. Missing Microsoft Visual C++ Redistributable
echo 2. Incompatible Qt version
echo 3. Missing additional Qt dependencies
echo 4. OpenCV library conflicts
echo.

goto end

:error
echo.
echo ? Critical files missing! Please run the fix_dlls.bat script first.
echo.

:end
pause