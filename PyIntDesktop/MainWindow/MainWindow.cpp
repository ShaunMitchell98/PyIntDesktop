#include "MainWindow.h"
#include "../About/About.h"
#include "../Resource.h"
#include <windowsx.h>
#include "../TextBox/TextBox.h"
#include "../Button/Button.h"
#include "../Interpreter/Interpreter.h"
#include "../FileReader/FileReader.h"
#include "../FileWriter/FileWriter.h"
#include "../CharConverter/CharConverter.h"

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
            case ID_OPEN_FILE:
            {
                MainWindow* mainWindow = (MainWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
                mainWindow->OpenFile();
                return 0;
            }
            case ID_FILE_SAVE: 
            {
                MainWindow* mainWindow = (MainWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
                mainWindow->SaveFile();
                return 0;

            }
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
    wcex.hbrBackground = CreateSolidBrush(RGB(26, 26, 26));
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PYINT_DESKTOP);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL MainWindow::InitWindow(HINSTANCE hInstance, int nCmdShow, WCHAR* szWindowClass, WCHAR* szTitle)
{
    HMENU hMenu = LoadMenu(hInstance, L"IDC_WINDOWSPROJECT1");
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, hMenu, hInstance, this);

    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

void MainWindow::SetUpChildWindows( HWND hWnd) {
    TextBox textBox;
    Button button;
    _hWndInputTextBox = textBox.InitWindow((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), hWnd, 0, 0, 500);
    _hWndOutputTextBox = textBox.InitWindow((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), hWnd, 0, 600, 100);
    _hWndButton = button.InitWindow((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), hWnd);
}

void MainWindow::OpenFile()
{
    FileReader fileReader;
    char* fileText = fileReader.ReadFile();
    CharConverter charConverter;
    LPWSTR lpwstrText = charConverter.CharPointerToLpwstr(fileText, CP_UTF8);
    _filePath = fileReader.GetFilePath();
    SetWindowText(_hWndInputTextBox, lpwstrText);
}

void MainWindow::SaveFile()
{
    int textLength = GetWindowTextLength(_hWndInputTextBox);
    LPTSTR input = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, textLength * 2);

    if (input != NULL) {
        GetWindowText(_hWndInputTextBox, input, textLength + 1);

        CharConverter charConverter;
        char* fileText = charConverter.LptstrToCharPointer(input, CP_UTF8);

        FileWriter fileWriter;
        fileWriter.WriteFile(fileText, _filePath);
        _filePath = fileWriter.GetFilePath();
    }
}

void MainWindow::InterpretCode() 
{
    int textLength = GetWindowTextLength(_hWndInputTextBox);
    LPTSTR input = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, textLength*2);

    if (input != NULL) {
        GetWindowText(_hWndInputTextBox, input, textLength + 1);

        Interpreter interpreter;
        LPWSTR output = interpreter.Interpret(input);
        SetWindowText(_hWndOutputTextBox, output);
    }

    HeapFree(GetProcessHeap(), 0, input);
}

