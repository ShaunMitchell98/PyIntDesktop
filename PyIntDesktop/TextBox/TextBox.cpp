#include "TextBox.h"

HWND TextBox::InitWindow(HINSTANCE hInstance, HWND parentWindow)
{
    HWND textBox = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL | ES_WANTRETURN,
        0, 0, 700, 100, parentWindow, nullptr, hInstance, 0);

    if (!textBox)
    {
        return textBox;
    }

    return textBox;
}