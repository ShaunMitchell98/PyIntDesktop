#pragma once

#include "../framework.h"

class MainWindow
{
    HINSTANCE _hInstance;

public:
    MainWindow(HINSTANCE hInstance);
    ATOM RegisterWindowClass(HINSTANCE hInstance, WCHAR* szWindowClass);
    BOOL InitWindow(HINSTANCE hInstance, int nCmdShow, WCHAR* szWindowClass, WCHAR* szTitle);
};

