#pragma once

#include "../framework.h"

class MainWindow
{
    HINSTANCE _hInstance;
    HWND _hWndInputTextBox;
    HWND _hWndOutputTextBox;
    HWND _hWndButton;

public:
    MainWindow(HINSTANCE hInstance);
    ATOM RegisterWindowClass(HINSTANCE hInstance, WCHAR* szWindowClass);
    BOOL InitWindow(HINSTANCE hInstance, int nCmdShow, WCHAR* szWindowClass, WCHAR* szTitle);
    void SetUpChildWindows(HWND hWnd);
    void InterpretCode();

private:

};