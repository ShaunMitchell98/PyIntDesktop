#include "MainWindow.h"
#include "../../Resource.h"
#include <windowsx.h>
#include "../TextBox/TextBox.h"
#include "../../Infrastructure/Interpreter/Interpreter.h"
#include "../../IO/FileWriter/FileWriter.h"
#include "../../IO/FileReader/FileReader.h"
#include "../../Infrastructure/CharConverter/CharConverter.h"
#include <shobjidl.h>

namespace UI 
{
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
        case WM_CTLCOLOREDIT:
        {
            HDC hdc = (HDC)wParam;
            HWND hEditControl = (HWND)lParam;
            MainWindow* mainWindow = (MainWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            return mainWindow->StyleEditControl(hEditControl, hdc);
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
            case ID_RUN: {
                MainWindow* mainWindow = (MainWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
                mainWindow->InterpretCode();
                return 0;
            }
            case IDM_EXIT:
                DestroyWindow(hWnd);
                return 0;
            }

            wmId = HIWORD(wParam);
            switch (wmId)
            {
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
        _inputTextBox = NULL;
        _outputTextBox = NULL;
        _filePath = NULL;
    }

    MainWindow::~MainWindow() 
    {
        CoTaskMemFree(_filePath);
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

    void MainWindow::SetUpChildWindows(HWND hWnd) {
        _inputTextBox = new TextBox((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), hWnd, 0, 0, 500);
        _outputTextBox = new TextBox((HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), hWnd, 0, 550, 100);
    }

    void MainWindow::OpenFile()
    {
        IO::FileReader fileReader;
        char* fileText = fileReader.ReadFile();
        Infrastructure::CharConverter charConverter;
        LPWSTR lpwstrText = charConverter.CharPointerToLpwstr(fileText, CP_UTF8);
        _filePath = fileReader.GetFilePath();
        SetWindowText(_inputTextBox->GetHandle(), lpwstrText);
    }

    void MainWindow::SaveFile()
    {
        int textLength = GetWindowTextLength(_inputTextBox->GetHandle());
        LPTSTR input = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, textLength * 2);

        if (input != NULL) {
            GetWindowText(_inputTextBox->GetHandle(), input, textLength + 1);

            Infrastructure::CharConverter charConverter;
            char* fileText = charConverter.LptstrToCharPointer(input, CP_UTF8);

            IO::FileWriter fileWriter;
            fileWriter.WriteFile(fileText, _filePath);
            _filePath = fileWriter.GetFilePath();
        }
    }

    void MainWindow::InterpretCode()
    {
        int textLength = GetWindowTextLength(_inputTextBox->GetHandle());
        LPTSTR input = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, textLength * 2);

        if (input != NULL) {
            GetWindowText(_inputTextBox->GetHandle(), input, textLength + 1);

            Infrastructure::Interpreter interpreter;
            LPWSTR output = interpreter.Interpret(input);
            SetWindowText(_outputTextBox->GetHandle(), output);
        }

        HeapFree(GetProcessHeap(), 0, input);
    }

    INT_PTR MainWindow::StyleEditControl(HWND hEditControl, HDC hdc) 
    {
        if (hEditControl == _inputTextBox->GetHandle())
        {
            return _inputTextBox->PaintWindow(hdc);
        }
        else if (hEditControl == _outputTextBox->GetHandle()) 
        {
            return _outputTextBox->PaintWindow(hdc);
        }
    }
}


