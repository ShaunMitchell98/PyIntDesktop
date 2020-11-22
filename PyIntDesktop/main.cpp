//main.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "MainWindow/MainWindow.h"
#include "main.h"

#define MAX_LOADSTRING 100

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
    WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PYINT_DESKTOP, szWindowClass, MAX_LOADSTRING);

    MainWindow* mainWindow = new MainWindow(hInstance);

    mainWindow->RegisterWindowClass(hInstance, szWindowClass);


    // Perform application initialization:
    if (!mainWindow->InitWindow(hInstance, nCmdShow, szWindowClass, szTitle))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PYINT_DESKTOP));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}