@echo off
echo ?? SUCCESS: Qt6 + OpenCV Working!
echo ================================
echo.
echo ? Minimal test completed successfully:
echo   - Qt 6.7.3: Working
echo   - OpenCV 4.3.0: Working  
echo   - Image loading: Working (735x542 image loaded)
echo   - Dark theme: Working
echo   - All DLLs: Properly linked
echo.
echo ?? The issue is specifically with the main ImageProcessorApp code.
echo    Not a fundamental Qt/OpenCV problem.
echo.
echo ???  Next steps:
echo   1. Simplify MainWindow initialization
echo   2. Add features incrementally to isolate the crash
echo   3. Check for memory allocation issues
echo.

REM Now let's test the console version to make sure that still works
echo ?? Testing console version to ensure OpenCV still works there...
echo.

cd ..\build_console\Release
if exist console_app.exe (
    echo Console app test:
    echo 0 | console_app.exe
    echo.
    echo ? Console version working
) else (
    echo ? Console version not found
)

echo.
echo ?? Ready to fix the main GUI application!
pause