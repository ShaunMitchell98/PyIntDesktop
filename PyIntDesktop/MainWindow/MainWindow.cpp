#include "MainWindow.h"
#include "../About/About.h"
#include "../Resource.h"
#include <windowsx.h>
#include "../TextBox/TextBox.h"
#include "../Button/Button.h"
#include "../Interpreter/Interpreter.h"

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE: {
        CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
        MainWindow* mainWindow = (MainWindow*)cs->lpCreateParams;
        mainWindow->SetUpChildWindows(hWnd);
        SetLastError(0);
        if (SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)mainWindow) == 0) {
            if (GetLastError() != 0) {
                return FALSE;
            }
        }
        return 0;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
            case IDM_ABOUT:
                DialogBox((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                return 0;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                return 0;
        }

        wmId = HIWORD(wParam);
        switch (wmId)
        {
            case BN_CLICKED: {
                MainWindow* mainWindow = (MainWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
                mainWindow->InterpretCode();
                return 0;
            }
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

MainWindow::MainWindow(HINSTANCE hInstance) 
{
    _hInstance = hInstance;
    _hWndInputTextBox = NULL;
    _hWndOutputTextBox = NULL;
    _hWndButton = NULL;
}

ATOM MainWindow::RegisterWindowClass(HINSTANCE hInstance, WCHAR* szWindowClass)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = (WNDPROC)WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PYINT_DESKTOP));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PYINT_DESKTOP);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL MainWindow::InitWindow(HINSTANCE hInstance, int nCmdShow, WCHAR* szWindowClass, WCHAR* szTitle)
{
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, this);

    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

void MainWindow::SetUpChildWindows(HWND hWnd) {
    TextBox textBox;
    Button button;
    _hWndInputTextBox = textBox.InitWindow((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), hWnd, 0, 0);
    _hWndOutputTextBox = textBox.InitWindow((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), hWnd, 0, 200);
    _hWndButton = button.InitWindow((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), hWnd);
}

void MainWindow::InterpretCode() 
{
    LPTSTR input = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 25);

    if (input != NULL) {
        GetWindowText(_hWndInputTextBox, input, 12);

        Interpreter interpreter;
        LPWSTR output = interpreter.Interpret(input);
        SetWindowText(_hWndOutputTextBox, output);
    }

    HeapFree(GetProcessHeap(), 0, input);
}

