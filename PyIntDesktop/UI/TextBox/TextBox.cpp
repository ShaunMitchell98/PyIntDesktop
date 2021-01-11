#include "TextBox.h"

namespace UI
{
    TextBox::TextBox(HINSTANCE hInstance, HWND parentWindow, int x, int y, int height)
    {
        _x = x;
        _y = y;
        _height = height;
        _width = 700;

        _hwnd = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_LEFT | ES_WANTRETURN | ES_AUTOVSCROLL,
            x, y, _width, height, parentWindow, nullptr, hInstance, 0);
  
    }

    INT_PTR TextBox::PaintWindow(HDC hdc) 
    {
        DWORD backgroundColor = RGB(20, 20, 20);
        SetBkColor(hdc, backgroundColor);
        SetTextColor(hdc, RGB(255, 255, 255));
        return (INT_PTR)CreateSolidBrush(backgroundColor);
    }

    HWND TextBox::GetHandle() 
    {
        return _hwnd;
    }
}