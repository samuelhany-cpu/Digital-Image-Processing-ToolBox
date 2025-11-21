#include <Windows.h>
#include "ImageProcessingGUI.h"
#include <iostream>
#include <io.h>
#include <fcntl.h>

// Function to allocate a console for GUI application
void AllocateConsole() {
    AllocConsole();
    
    // Redirect stdout, stdin, stderr to console
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
    freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
    
    // Sync with iostream
    std::ios::sync_with_stdio(true);
    std::wcout.clear();
    std::cout.clear();
    std::wcerr.clear();
    std::cerr.clear();
    std::wcin.clear();
    std::cin.clear();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Allocate console for output from Lab functions
    AllocateConsole();
    SetConsoleTitle(L"Image Processing Lab - Console Output");
    
    // Initialize COM (needed for some OpenCV operations)
    CoInitialize(nullptr);
    
    // Create and initialize GUI
    ImageProcessingGUI gui(hInstance);
    
    if (!gui.Initialize()) {
        MessageBox(nullptr, L"Failed to initialize GUI application!", L"Error", MB_OK | MB_ICONERROR);
        CoUninitialize();
        return -1;
    }
    
    // Show welcome message in console
    std::cout << "=== Image Processing Lab - GUI Version ===" << std::endl;
    std::cout << "Console output will appear here when you run lab functions." << std::endl;
    std::cout << "Image windows will open when processing images." << std::endl;
    std::cout << "===========================================" << std::endl << std::endl;
    
    // Run the GUI message loop
    gui.Run();
    
    // Cleanup
    FreeConsole();
    CoUninitialize();
    return 0;
}