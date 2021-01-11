#pragma once

#include "../../framework.h"
#include "../TextBox/TextBox.h"

namespace UI
{
    class MainWindow
    {
        HINSTANCE _hInstance;
        TextBox* _inputTextBox;
        TextBox* _outputTextBox;
        PWSTR _filePath;

    public:
        MainWindow(HINSTANCE hInstance);
        ~MainWindow();
        ATOM RegisterWindowClass(HINSTANCE hInstance, WCHAR* szWindowClass);
        BOOL InitWindow(HINSTANCE hInstance, int nCmdShow, WCHAR* szWindowClass, WCHAR* szTitle);
        void SetUpChildWindows(HWND hWnd);
        void OpenFile();
        void SaveFile();
        void InterpretCode();
        INT_PTR StyleEditControl(HWND hEditControl, HDC hdc);
    };
}

