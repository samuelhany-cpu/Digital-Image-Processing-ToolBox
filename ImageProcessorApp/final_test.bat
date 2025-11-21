@echo off
echo ?? Final Application Test
echo =======================
echo.

cd build\Release

echo Checking if ImageProcessorApp.exe exists...
if not exist ImageProcessorApp.exe (
    echo ? ImageProcessorApp.exe not found!
    goto end
)
echo ? ImageProcessorApp.exe found
echo.

echo Testing application...
echo Press Ctrl+C if the app window opens successfully!
echo Otherwise, watch for crash or error code.
echo.

REM Try to run with error capture
ImageProcessorApp.exe 2>&1
set EXIT_CODE=%ERRORLEVEL%

echo.
echo Exit code: %EXIT_CODE%
echo.

if %EXIT_CODE% EQU 0 (
    echo ? Application executed successfully!
) else if %EXIT_CODE% EQU -1073741819 (
    echo ? Still crashing with access violation!
    echo    The fix didn't solve the issue yet.
) else (
    echo ??  Application exited with code: %EXIT_CODE%
)

cd ..\..

:end
pause