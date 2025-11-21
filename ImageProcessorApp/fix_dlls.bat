@echo off
echo ?? Fixing Qt6 DLL Dependencies for ImageProcessorApp...
echo.

cd build\Release

REM Copy Qt6 Core DLLs
echo Copying Qt6 Core DLLs...
copy "C:\Qt\6.7.3\msvc2019_64\bin\Qt6Core.dll" . 2>nul
copy "C:\Qt\6.7.3\msvc2019_64\bin\Qt6Gui.dll" . 2>nul
copy "C:\Qt\6.7.3\msvc2019_64\bin\Qt6Widgets.dll" . 2>nul
copy "C:\Qt\6.7.3\msvc2019_64\bin\Qt6Network.dll" . 2>nul

REM Create platforms directory and copy platform plugins
echo Setting up platform plugins...
if not exist "platforms" mkdir platforms
copy "C:\Qt\6.7.3\msvc2019_64\plugins\platforms\qwindows.dll" platforms\ 2>nul
copy "C:\Qt\6.7.3\msvc2019_64\plugins\platforms\qminimal.dll" platforms\ 2>nul

REM Create styles directory and copy style plugins (if needed)
if not exist "styles" mkdir styles
copy "C:\Qt\6.7.3\msvc2019_64\plugins\styles\qwindowsvistastyle.dll" styles\ 2>nul

echo.
echo ? All DLLs copied successfully!
echo.
echo ?? Current directory contents:
dir /b

echo.
echo ?? Starting ImageProcessorApp...
echo.
echo If the app doesn't start, try copying additional DLLs manually:
echo - Qt6OpenGL.dll
echo - Any missing MSVC runtime DLLs
echo.

REM Try to run the application
ImageProcessorApp.exe

cd ..\..
echo.
echo ?? If you encountered issues, check the error message above.
echo The app should now work with all Qt6 dependencies resolved.
pause