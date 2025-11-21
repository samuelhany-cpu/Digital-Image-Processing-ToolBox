@echo off
echo ?? Checking ImageProcessorApp Dependencies...
echo.

cd build\Release

REM Check if all DLLs are present
echo ?? DLL Checklist:
if exist "Qt6Core.dll" (echo ? Qt6Core.dll) else (echo ? Qt6Core.dll MISSING)
if exist "Qt6Gui.dll" (echo ? Qt6Gui.dll) else (echo ? Qt6Gui.dll MISSING)
if exist "Qt6Widgets.dll" (echo ? Qt6Widgets.dll) else (echo ? Qt6Widgets.dll MISSING)
if exist "opencv_world430.dll" (echo ? opencv_world430.dll) else (echo ? opencv_world430.dll MISSING)
if exist "platforms\qwindows.dll" (echo ? platforms\qwindows.dll) else (echo ? platforms\qwindows.dll MISSING)

echo.
echo ?? Attempting to run ImageProcessorApp.exe...
echo.

REM Try to start the application
start "" ImageProcessorApp.exe

echo.
echo ?? Application started! If you see any error dialogs:
echo.
echo Common solutions:
echo 1. Install Microsoft Visual C++ Redistributable 2019/2022
echo 2. Copy missing MSVC runtime DLLs (vcruntime140.dll, msvcp140.dll, etc.)
echo 3. Check Windows Event Viewer for detailed error information
echo.
echo If the app opens successfully, try loading the translation.jpg image!

cd ..\..
pause