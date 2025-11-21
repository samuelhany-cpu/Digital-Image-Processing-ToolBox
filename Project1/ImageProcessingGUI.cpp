#include "ImageProcessingGUI.h"
#include <iostream>
#include <sstream>

// Static pointer to current instance for window procedure
static ImageProcessingGUI* g_pGUI = nullptr;

ImageProcessingGUI::ImageProcessingGUI(HINSTANCE hInst) : hInstance(hInst), hWnd(nullptr) {
    g_pGUI = this;
}

ImageProcessingGUI::~ImageProcessingGUI() {
    if (hWnd) {
        DestroyWindow(hWnd);
    }
}

bool ImageProcessingGUI::Initialize() {
    // Register window class
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = WINDOW_CLASS_NAME;
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        return false;
    }

    // Create main window
    hWnd = CreateWindowEx(
        0,
        WINDOW_CLASS_NAME,
        L"Image Processing Lab - GUI Version",
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX, // Fixed size window
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 400,
        nullptr, nullptr, hInstance, nullptr
    );

    if (!hWnd) {
        return false;
    }

    CreateControls();
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    return true;
}

void ImageProcessingGUI::CreateControls() {
    // Title label
    hTitleLabel = CreateWindowA(
        "STATIC", "Digital Image Processing Toolbox",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        50, 20, 400, 30,
        hWnd, (HMENU)ID_STATIC_TITLE, hInstance, nullptr
    );

    // Instructions label
    hInstructionsLabel = CreateWindowA(
        "STATIC", "Select a lab to run image processing operations:",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        50, 60, 400, 20,
        hWnd, (HMENU)ID_STATIC_INSTRUCTIONS, hInstance, nullptr
    );

    // Lab buttons
    hButtonLab1 = CreateWindowA(
        "BUTTON", "Lab 1 - Image Counter",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        150, 100, 200, 40,
        hWnd, (HMENU)ID_BUTTON_LAB1, hInstance, nullptr
    );

    hButtonLab2 = CreateWindowA(
        "BUTTON", "Lab 2 - Image Info & Display",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        150, 150, 200, 40,
        hWnd, (HMENU)ID_BUTTON_LAB2, hInstance, nullptr
    );

    hButtonLab3 = CreateWindowA(
        "BUTTON", "Lab 3 - Image Processing",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        150, 200, 200, 40,
        hWnd, (HMENU)ID_BUTTON_LAB3, hInstance, nullptr
    );

    hButtonLab4 = CreateWindowA(
        "BUTTON", "Lab 4 - Advanced Processing",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        150, 250, 200, 40,
        hWnd, (HMENU)ID_BUTTON_LAB4, hInstance, nullptr
    );

    // Exit button
    hButtonExit = CreateWindowA(
        "BUTTON", "Exit",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        200, 310, 100, 30,
        hWnd, (HMENU)ID_BUTTON_EXIT, hInstance, nullptr
    );

    // Set font for all controls
    HFONT hFont = CreateFontA(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, VARIABLE_PITCH, "Segoe UI");

    SendMessage(hTitleLabel, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(hInstructionsLabel, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(hButtonLab1, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(hButtonLab2, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(hButtonLab3, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(hButtonLab4, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(hButtonExit, WM_SETFONT, (WPARAM)hFont, TRUE);

    // Make title bold
    HFONT hBoldFont = CreateFontA(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, VARIABLE_PITCH, "Segoe UI");
    SendMessage(hTitleLabel, WM_SETFONT, (WPARAM)hBoldFont, TRUE);
}

void ImageProcessingGUI::HandleButtonClick(int buttonId) {
    try {
        int result = 0;
        std::wstring message;

        switch (buttonId) {
        case ID_BUTTON_LAB1:
            ShowMessage(L"Running Lab 1 - Image Counter...\nCheck console for output and image windows.");
            result = Lab1();
            message = L"Lab 1 completed with result code: " + std::to_wstring(result);
            ShowMessage(message, L"Lab 1 Complete");
            break;

        case ID_BUTTON_LAB2:
            ShowMessage(L"Running Lab 2 - Image Info & Display...\nCheck console for output and image windows.");
            result = Lab2();
            message = L"Lab 2 completed with result code: " + std::to_wstring(result);
            ShowMessage(message, L"Lab 2 Complete");
            break;

        case ID_BUTTON_LAB3:
            ShowMessage(L"Running Lab 3 - Image Processing...\nCheck console for output and image windows.");
            result = Lab3();
            message = L"Lab 3 completed with result code: " + std::to_wstring(result);
            ShowMessage(message, L"Lab 3 Complete");
            break;

        case ID_BUTTON_LAB4:
            ShowMessage(L"Running Lab 4 - Advanced Processing...\nCheck console for output and image windows.");
            result = Lab4();
            message = L"Lab 4 completed with result code: " + std::to_wstring(result);
            ShowMessage(message, L"Lab 4 Complete");
            break;

        case ID_BUTTON_EXIT:
            PostQuitMessage(0);
            break;
        }
    }
    catch (...) {
        ShowMessage(L"An error occurred while running the lab function.", L"Error");
    }
}

void ImageProcessingGUI::ShowMessage(const std::wstring& message, const std::wstring& title) {
    MessageBox(hWnd, message.c_str(), title.c_str(), MB_OK | MB_ICONINFORMATION);
}

void ImageProcessingGUI::Run() {
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK ImageProcessingGUI::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_COMMAND:
        if (g_pGUI && HIWORD(wParam) == BN_CLICKED) {
            g_pGUI->HandleButtonClick(LOWORD(wParam));
        }
        break;

    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}