#pragma once
#include <Windows.h>
#include <CommCtrl.h>
#include <string>
#include "counter.h"
#include "BW.h"
#include "DR.h"
#include "ImageProcesses.h"

// Window Class Name
#define WINDOW_CLASS_NAME L"ImageProcessingGUI"

// Control IDs
#define ID_BUTTON_LAB1 1001
#define ID_BUTTON_LAB2 1002
#define ID_BUTTON_LAB3 1003
#define ID_BUTTON_LAB4 1004
#define ID_BUTTON_EXIT 1005
#define ID_STATIC_TITLE 1006
#define ID_STATIC_INSTRUCTIONS 1007

class ImageProcessingGUI {
private:
    HWND hWnd;
    HWND hButtonLab1, hButtonLab2, hButtonLab3, hButtonLab4, hButtonExit;
    HWND hTitleLabel, hInstructionsLabel;
    HINSTANCE hInstance;

public:
    ImageProcessingGUI(HINSTANCE hInst);
    ~ImageProcessingGUI();
    
    bool Initialize();
    void Run();
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    
private:
    void CreateControls();
    void HandleButtonClick(int buttonId);
    void ShowMessage(const std::wstring& message, const std::wstring& title = L"Information");
};